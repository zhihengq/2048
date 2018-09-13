SRCDIR = src
INCDIR = include
TESTDIR = test
DOCDIR = docs
BUILDDIR = build
BINDIR = bin
LDFLAGS_GTEST = `gtest-config --ldflags --libs` -lgtest_main
override CXXFLAGS += -iquote $(INCDIR) -Wall -std=c++17 -fPIC
override LDFLAGS += -L$(BINDIR) -fPIC


### Phony Targets

all: $(patsubst %,$(BINDIR)/%, 2048_ncurses 2048_ncurses_random)

test: $(patsubst %,$(BINDIR)/$(TESTDIR)/%, auto_tests ncurses_test)
	LD_LIBRARY_PATH=$(BINDIR) $<

docs: Doxyfile
	doxygen Doxyfile

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/* $(DOCDIR)/*

.PHONY: all test docs clean


### Build Rule

# call BUILD_RULE,<group>,<src>,<headers>
define BUILD_RULE =
override $(strip $1) += $(BUILDDIR)/$(strip $2).o
$(BUILDDIR)/$(strip $2).o : $(SRCDIR)/$(strip $2).cc $(strip $(3:%=$(INCDIR)/%.h))
	@mkdir -p $$(@D)
	$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef


### Header Dependencies

H_ALL = tile grid game_state viewer generator player game ui/ncurses_viewer ui/ncurses_controller ai/random_generator ai/evaluation_function ai/weight_table_evaluator

H_TILE = tile
H_GRID = grid $(H_TILE)
H_GAME_STATE = game_state $(H_TILE) $(H_GRID)
H_VIEWER = viewer $(H_GAME_STATE)
H_GENERATOR = generator $(H_GAME_STATE)
H_PLAYER = player $(H_GAME_STATE)
H_GAME = game $(H_GAME_STATE) $(H_VIEWER) $(H_GENERATOR) $(H_PLAYER)
H_UI_NCURSES_VIEWER = ui/ncurses_viewer $(H_GAME_STATE) $(H_VIEWER)
H_UI_NCURSES_CONTROLLER = ui/ncurses_controller $(H_GAME_STATE) $(H_PLAYER) $(H_UI_NCURSES_VIEWER)
H_AI_RANDOM_GENERATOR = ai/random_generator $(H_GENERATOR)
H_AI_RANDOM_PLAYER = ai/random_player $(H_PLAYER)
H_AI_EVALUATION_FUNCTION = ai/evaluation_function $(H_GAME_STATE)
H_AI_WEIGHT_TABLE_EVALUATOR = ai/weight_table_evaluator $(H_AI_EVALUATION_FUNCTION)
H_AI_GRADIENT_LINEAR_4X4_WEIGHT_EVALUATOR = ai/gradient_linear_4x4_weight_evaluator $(H_AI_WEIGHT_TABLE_EVALUATOR)


### Objects

$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, tile, $(H_TILE)))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, grid, $(H_GRID)))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, game_state, $(H_GAME_STATE)))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, game, $(H_GAME)))

$(eval $(call BUILD_RULE, BOTS_OBJS, ai/random_generator, $(H_AI_RANDOM_GENERATOR)))
$(eval $(call BUILD_RULE, BOTS_OBJS, ai/random_player, $(H_AI_RANDOM_PLAYER)))
$(eval $(call BUILD_RULE, BOTS_OBJS, ai/weight_table_evaluator, $(H_AI_WEIGHT_TABLE_EVALUATOR)))
$(eval $(call BUILD_RULE, BOTS_OBJS, ai/gradient_linear_4x4_weight_evaluator, $(H_AI_GRADIENT_LINEAR_4X4_WEIGHT_EVALUATOR)))

$(eval $(call BUILD_RULE, NCURSES_OBJS, ui/ncurses_viewer, $(H_UI_NCURSES_VIEWER)))
$(eval $(call BUILD_RULE, NCURSES_OBJS, ui/ncurses_controller, $(H_UI_NCURSES_CONTROLLER)))

$(eval $(call BUILD_RULE, OTHER_OBJS, app/2048_ncurses, $(H_GAME) $(H_NCURSES_CONTROLLER) $(H_AI_RANDOM_GENERATOR)))
$(eval $(call BUILD_RULE, OTHER_OBJS, app/2048_ncurses_random, $(H_GAME) $(H_NCURSES_VIEWER) $(H_AI_RANDOM_GENERATOR) $(H_AI_RANDOM_PLAYER)))


### Executables

$(BINDIR)/libgamelogic.so : $(GAMELOGIC_OBJS)
	$(CXX) $(LDFLAGS) -shared $^ -o $@

$(BINDIR)/libbots.so : $(BOTS_OBJS)
	$(CXX) $(LDFLAGS) -shared $^ -o $@

$(BINDIR)/2048_ncurses : $(patsubst %,$(BUILDDIR)/%.o,app/2048_ncurses ai/random_generator) $(NCURSES_OBJS) | $(BINDIR)/libgamelogic.so
	$(CXX) $(LDFLAGS) -lgamelogic -lncurses $^ -o $@

$(BINDIR)/2048_ncurses_random : $(patsubst %,$(BUILDDIR)/%.o,app/2048_ncurses_random ui/ncurses_viewer) | $(BINDIR)/libgamelogic.so $(BINDIR)/libbots.so
	$(CXX) $(LDFLAGS) -lgamelogic -lbots -lncurses $^ -o $@


### Tests

$(BINDIR)/$(TESTDIR)/auto_tests : $(patsubst %,$(BUILDDIR)/$(TESTDIR)/%_test.o,tile grid game_state game evaluation_function gradient_linear_4x4_weight_evaluator) | $(BINDIR)/libgamelogic.so $(BINDIR)/libbots.so
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -lgamelogic -lbots $(LDFLAGS_GTEST) $^ -o $@

$(BINDIR)/$(TESTDIR)/ncurses_test : $(BUILDDIR)/$(TESTDIR)/ncurses_test.o $(NCURSES_OBJS) | $(BINDIR)/libgamelogic.so
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -lgamelogic -lncurses $^ -o $@

# rebuild all tests if API changes
$(BUILDDIR)/$(TESTDIR)/%_test.o : $(TESTDIR)/%_test.cc $(H_ALL:%=$(INCDIR)/%.h)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

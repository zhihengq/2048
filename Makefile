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

all: $(patsubst %,$(BINDIR)/%, ncurses/2048 ncurses/2048_random)

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
$(strip $1) += $(BUILDDIR)/$(strip $2).o
_DEPS = $(SRCDIR)/$(strip $2).cc $(strip $(3:%=$(INCDIR)/%.h))
$(BUILDDIR)/$(strip $2).o : $$(_DEPS)
	@mkdir -p $$(@D)
	$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef


### Header Dependencies

H_ALL  = tile grid game_state viewer generator player game
H_ALL += ui/ncurses_viewer ui/ncurses_controller
H_ALL += ai/random_generator ai/random_player
H_ALL += ai/eval/eval_func ai/eval/weight_table/weight_table
H_ALL += ai/eval/weight_table/gradient_linear_4x4
H_ALL += ai/eval/weight_table/gradient_exponential_4x4

H_TILE = tile
H_GRID = grid $(H_TILE)
H_GAME_STATE = game_state $(H_TILE) $(H_GRID)
H_VIEWER = viewer $(H_GAME_STATE)
H_GENERATOR = generator $(H_GAME_STATE)
H_PLAYER = player $(H_GAME_STATE)
H_GAME = game $(H_GAME_STATE) $(H_VIEWER) $(H_GENERATOR) $(H_PLAYER)
H_UI_NCURSESVIEWER = ui/ncurses_viewer $(H_GAME_STATE) $(H_VIEWER)
H_UI_NCURSESCONTROLLER  = ui/ncurses_controller
H_UI_NCURSESCONTROLLER += $(H_GAME_STATE) $(H_PLAYER) $(H_UI_NCURSESVIEWER)
H_AI_RANDOMGENERATOR = ai/random_generator $(H_GENERATOR)
H_AI_RANDOMPLAYER = ai/random_player $(H_PLAYER)
H_AI_EVAL_EVALFUNC = ai/eval/eval_func $(H_GAME_STATE)
H_AI_EVAL_WEIGHTTABLE_WEIGHTTABLE  = ai/eval/weight_table/weight_table
H_AI_EVAL_WEIGHTTABLE_WEIGHTTABLE += $(H_AI_EVAL_EVALFUNC)
H_AI_EVAL_WEIGHTTABLE_GRADIENTLINEAR4X4  = ai/eval/weight_table/gradient_linear_4x4
H_AI_EVAL_WEIGHTTABLE_GRADIENTLINEAR4X4 += $(H_AI_EVAL_WEIGHTTABLE_WEIGHTTABLE)
H_AI_EVAL_WEIGHTTABLE_GRADIENTEXPONENTIAL4X4  = ai/eval/weight_table/gradient_exponential_4x4
H_AI_EVAL_WEIGHTTABLE_GRADIENTEXPONENTIAL4X4 += $(H_AI_EVAL_WEIGHTTABLE_WEIGHTTABLE)


### Objects

$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, tile, $(H_TILE)))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, grid, $(H_GRID)))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, game_state, $(H_GAME_STATE)))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, game, $(H_GAME)))

_H = $(H_AI_RANDOMGENERATOR)
$(eval $(call BUILD_RULE, RANDOM_OBJS, ai/random_generator, $(_H)))
_H = $(H_AI_RANDOMPLAYER)
$(eval $(call BUILD_RULE, RANDOM_OBJS, ai/random_player, $(_H)))

_S = ai/eval/weight_table/weight_table
_H = $(H_AI_WEIGHT_TABLE_EVALUATOR)
$(eval $(call BUILD_RULE, BOTS_OBJS, $(_S), $(_H)))
_S = ai/eval/weight_table/gradient_linear_4x4
_H = $(H_AI_EVAL_WEIGHTTABLE_GRADIENTLINEAR4X4)
$(eval $(call BUILD_RULE, BOTS_OBJS, $(_S), $(_H)))
_S = ai/eval/weight_table/gradient_exponential_4x4
_H = $(H_AI_EVAL_WEIGHTTABLE_GRADIENTEXPONENTIAL4X4)
$(eval $(call BUILD_RULE, BOTS_OBJS, $(_S), $(_H)))

_H = $(H_UI_NCURSESVIEWER)
$(eval $(call BUILD_RULE, NCURSES_OBJS, ui/ncurses_viewer, $(_H)))
_H = $(H_UI_NCURSESCONTROLLER)
$(eval $(call BUILD_RULE, NCURSES_OBJS, ui/ncurses_controller, $(_H)))

_H = $(H_GAME) $(H_NCURSES_CONTROLLER) $(H_AI_RANDOMGENERATOR)
$(eval $(call BUILD_RULE, APP_OBJS, app/ncurses/2048, $(_H)))
_H  = $(H_GAME) $(H_NCURSES_VIEWER)
_H += $(H_AI_RANDOMGENERATOR) $(H_AI_RANDOMPLAYER)
$(eval $(call BUILD_RULE, APP_OBJS, app/ncurses/2048_random, $(_H)))


### Executables

$(BINDIR)/libgamelogic.so : $(GAMELOGIC_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -shared $^ -o $@

$(BINDIR)/libbots.so : $(BOTS_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -shared $^ -o $@

_DEPS  = $(patsubst %,$(BUILDDIR)/%.o,app/ncurses/2048 ai/random_generator)
_DEPS += $(NCURSES_OBJS) | $(BINDIR)/libgamelogic.so
$(BINDIR)/ncurses/2048 : $(_DEPS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -lgamelogic -lncurses $^ -o $@

_DEPS  = $(patsubst %,$(BUILDDIR)/%.o,app/ncurses/2048_random ui/ncurses_viewer)
_DEPS += $(RANDOM_OBJS) | $(BINDIR)/libgamelogic.so
$(BINDIR)/ncurses/2048_random : $(_DEPS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -lgamelogic -lncurses $^ -o $@


### Tests

AUTO_TESTS  = tile grid game_state game
AUTO_TESTS += ai/eval/eval_func
AUTO_TESTS += ai/eval/weight_table/gradient_linear_4x4
AUTO_TESTS += ai/eval/weight_table/gradient_exponential_4x4
_DEPS  = $(patsubst %,$(BUILDDIR)/$(TESTDIR)/%_test.o, $(AUTO_TESTS))
_DEPS += | $(BINDIR)/libgamelogic.so $(BINDIR)/libbots.so
$(BINDIR)/$(TESTDIR)/auto_tests : $(_DEPS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -lgamelogic -lbots $(LDFLAGS_GTEST) $^ -o $@

_DEPS  = $(BUILDDIR)/$(TESTDIR)/ui/ncurses_test.o $(NCURSES_OBJS)
_DEPS += | $(BINDIR)/libgamelogic.so
$(BINDIR)/$(TESTDIR)/ncurses_test : $(_DEPS)
	@mkdir -p $(@D)
	$(CXX) $(LDFLAGS) -lgamelogic -lncurses $^ -o $@

# rebuild all tests if API changes
$(BUILDDIR)/$(TESTDIR)/%_test.o : $(TESTDIR)/%_test.cc $(H_ALL:%=$(INCDIR)/%.h)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

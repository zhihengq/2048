SRCDIR = src
INCDIR = include
TESTDIR = test
DOCDIR = docs
BUILDDIR = build
BINDIR = bin
LDFLAGS_GTEST = `gtest-config --ldflags --libs` -lgtest_main
override CXXFLAGS += -iquote $(INCDIR) -Wall -std=c++17 -fPIC
override LDFLAGS += -L$(BINDIR) -fPIC

all: $(BINDIR)/libgamelogic.so

test: $(BINDIR)/test_suite
	LD_LIBRARY_PATH=$(BINDIR) $<

docs: Doxyfile
	doxygen Doxyfile

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/* $(DOCDIR)/*

.PHONY: all test docs clean

# call BUILD_RULE,<group>,<src>,<headers>
define BUILD_RULE =
override $(strip $1) += $(BUILDDIR)/$(strip $2).o
$(BUILDDIR)/$(strip $2).o : $(SRCDIR)/$(strip $2).cc $(strip $(3:%=$(INCDIR)/%.h))
	@mkdir -p $$(@D)
	$(CXX) $(CXXFLAGS) -c $$< -o $$@
endef


# Targets

$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, tile, tile))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, grid, tile grid))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, game_state, tile grid game_state))
$(eval $(call BUILD_RULE, GAMELOGIC_OBJS, game, tile grid game_state game viewer generator player))

$(eval $(call BUILD_RULE, VIEWER_OBJS, ncurses_viewer, ncurses_viewer tile grid game_state))

$(BINDIR)/libgamelogic.so: $(GAMELOGIC_OBJS)
	$(CXX) $(LDFLAGS) -shared $^ -o $@


# Tests

$(BINDIR)/test_suite : $(patsubst %.cc,$(BUILDDIR)/%.o,$(wildcard $(TESTDIR)/*_test.cc)) | all
	$(CXX) $(LDFLAGS) -lgamelogic $(LDFLAGS_GTEST) $^ -o $@

$(BUILDDIR)/$(TESTDIR)/%_test.o : $(TESTDIR)/%_test.cc $(BUILDDIR)/%.o
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

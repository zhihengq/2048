SRCDIR = src
INCDIR = include
TESTDIR = test
DOCDIR = docs
BUILDDIR = build
BINDIR = bin
CXXFLAGS += -I$(INCDIR) -Wall -std=c++17
LDFLAGS += -fpic
LDFLAGS_GTEST = `gtest-config --ldflags --libs` -lgtest_main

_HEADERS = tile grid game_state
HEADERS = $(patsubst %,$(INCDIR)/%.h,$(_HEADERS))

_OBJS = tile grid game_state
OBJS = $(patsubst %,$(BUILDDIR)/%.o,$(_OBJS))

_TESTS = tile grid game_state
TESTS = $(patsubst %,$(BUILDDIR)/%_test.o,$(_TESTS))

all: # empty

test: $(BINDIR)/test_suite
	$(BINDIR)/test_suite

docs: Doxyfile
	doxygen Doxyfile

clean:
	rm -rf $(BUILDDIR)/* $(BINDIR)/* $(DOCDIR)/*

.PHONY: all test docs clean

$(BINDIR)/test_suite: $(TESTS) $(OBJS)
	$(CXX) $(LDFLAGS) $(LDFLAGS_GTEST) $^ -o $@

$(BUILDDIR)/%_test.o: $(TESTDIR)/%_test.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

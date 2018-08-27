SRCDIR = src
INCDIR = include
BUILDDSIR = build
BINDIR = bin
CPPFLAGS += -I$(INCDIR)
CXXFLAGS += -Wall -std=c++17

all:

clean:
	rm -r build

.PHONY: all clean

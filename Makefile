CXX=g++

LD=$(CXX)
AR=ar

DEBUG=-ggdb
CXXFLAGS=-O3 -Wall $(DEBUG) $(INCPATHS) -std=c++20
LDFLAGS=-O3 $(DEBUG) $(LIBPATHS) -L. -lreadline

LIBSOURCES=
LIBOBJS=$(LIBSOURCES:%.cpp=%.o)

MAIN=interpreter.cpp
TARGETS=$(MAIN:%.cpp=%)

.PHONY:	all clean
.SUFFIXES: .cpp .o

all: $(TARGETS)

$(TARGETS): %: %.o
	$(LD) $^ -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGETS)

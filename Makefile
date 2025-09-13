CXX=g++

LD=$(CXX)
AR=ar

DEBUG=-ggdb
CXXFLAGS=-O3 -Wall $(DEBUG) $(INCPATHS) -std=c++20
LDFLAGS=-O3 $(DEBUG) $(LIBPATHS) -L. -lreadline

LIBSOURCES=readline.cpp
LIBOBJS=$(LIBSOURCES:%.cpp=%.o)

MAIN=interpreter.cpp
TARGET=$(MAIN:%.cpp=%)

.PHONY:	all clean
.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): %: %.o libpilseung.a
	$(LD) $^ -o $@ $(LDFLAGS)

libpilseung.a: $(LIBOBJS)
	$(AR) rcs $@ $^

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)

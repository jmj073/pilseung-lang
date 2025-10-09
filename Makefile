CXX=g++
LD=$(CXX)
AR=ar

DEBUG=-ggdb
CXXFLAGS=-O3 -Wall $(DEBUG) $(INCPATHS) -std=c++23
LDFLAGS=-O3 $(DEBUG) $(LIBPATHS)

BUILD_DIR=build

LIBSOURCES=readline.cpp tokenize.cpp parse.cpp ast.cpp memory.cpp eval.cpp
LIBOBJS=$(addprefix $(BUILD_DIR)/,$(LIBSOURCES:%.cpp=%.o))

MAIN=interpreter.cpp
TARGET=$(MAIN:%.cpp=%)

DEPS=$(BUILD_DIR)/.deps
STATIC_LIB=$(BUILD_DIR)/libpilseung.a

.PHONY: all clean
.SUFFIXES: .cpp .o

all: $(TARGET)

# 최종 실행 파일은 build 밖에 생성됨
$(TARGET): $(BUILD_DIR)/interpreter.o $(STATIC_LIB)
	$(LD) $^ -o $@ $(LDFLAGS)

# 정적 라이브러리 생성 (build 안에 위치)
$(STATIC_LIB): $(LIBOBJS)
	$(AR) rcs $@ $^

# .cpp -> .o 규칙 (build 안에 오브젝트 생성)
$(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 의존성 파일 생성
$(DEPS): *.cpp *.h | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -MM *.cpp | sed 's|^\(.*\)\.o[ :]*|$(BUILD_DIR)/\1.o : |g' > $(DEPS)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)

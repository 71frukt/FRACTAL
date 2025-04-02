CC         =  g++
OPT_FLAG   = -mavx2 -pg -g
OPT_LEVEL ?= -O3
CFLAGS     = -Wall -Wextra -std=c++17 -Ilib
SFMLFLAGS  = -lsfml-graphics -lsfml-window -lsfml-system

LDFLAGS  = -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual                \
 -Wchar-subscripts -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal -Wformat-nonliteral -Wformat-security -Wformat-signedness               \
 -Wformat=2 -Winline -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion         \
 -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default            \
 -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing                  \
 -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer              \
 -Wlarger-than=81920 -Wstack-usage=81920 -pie -fPIE -Werror=vla -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr


SRC_DIR   = src
LIB_DIR   = lib
BIN_DIR   = bin
BUILD_DIR = build

SRC_CPP  = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_CPP  = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRC_CPP))
TARGET   = $(BUILD_DIR)/mandelbrot


ifneq (,$(filter test_benchmark,$(MAKECMDGOALS)))
  CFLAGS += -D TEST_BENCHMARK -D NO_GRAPHICS
endif

ifneq (,$(filter no_graphics,$(MAKECMDGOALS)))
  CFLAGS += -D NO_GRAPHICS
endif


ifneq (,$(filter optimize_O0,$(MAKECMDGOALS)))
  OPT_LEVEL = -O0
endif

ifneq (,$(filter optimize_O1,$(MAKECMDGOALS)))
  OPT_LEVEL = -O1
endif

ifneq (,$(filter optimize_O2,$(MAKECMDGOALS)))
  OPT_LEVEL = -O2
endif

ifneq (,$(filter optimize_O3,$(MAKECMDGOALS)))
  OPT_LEVEL = -O3
endif

all: $(TARGET)

$(TARGET): $(OBJ_CPP) | $(BUILD_DIR)
	$(CC) $(OBJ_CPP) -o $@ $(SFMLFLAGS) $(LDFLAGS)

$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CC) $(OPT_FLAG) $(OPT_LEVEL) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -f $(BIN_DIR)/*

rebuild: clean all

test_benchmark no_graphics optimize_O0 optimize_O1 optimize_O2 optimize_O3: rebuild

.PHONY: all clean rebuild
# Компилятор и флаги
CC       = g++
CFLAGS   = -Wall -Wextra -std=c++17 -Ilib
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Дополнительные флаги линковки
LDFLAGS  = -Wshadow -Winit-self -Wredundant-decls -Wcast-align -Wundef      \
           -Wfloat-equal -Winline -Wunreachable-code -Wmissing-declarations  \
           -Wmissing-include-dirs -Wswitch-enum -Wswitch-default -Weffc++     \
           -Wmain -Wextra -Wall -g -pipe -fexceptions -Wcast-qual              \
           -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-security      \
           -Wformat=2 -Wignored-qualifiers -Wlogical-op                          \
           -Wno-missing-field-initializers -Wnon-virtual-dtor                     \
           -Woverloaded-virtual -Wpointer-arith -Wsign-promo                       \
           -Wstrict-aliasing -Wstrict-null-sentinel -Wtype-limits                   \
           -Wwrite-strings -Werror=vla -D_DEBUG -D_EJUDGE_CLIENT_SIDE

# Структура папок
SRC_DIR  = src
LIB_DIR  = lib
BIN_DIR  = bin
BUILD_DIR = build

# Исходные файлы и цели
SRC_CPP  = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_CPP  = $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(SRC_CPP))
TARGET   = $(BUILD_DIR)/prog

# Основная цель
all: $(TARGET)

# Линковка
$(TARGET): $(OBJ_CPP) | $(BUILD_DIR)
	$(CC) $(OBJ_CPP) -o $@ $(SFMLFLAGS) $(LDFLAGS)

# Компиляция .cpp в .o
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Создание папок при необходимости
$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Очистка
clean:
	rm -f $(BIN_DIR)/* $(BUILD_DIR)/*

rebuild: clean all

.PHONY: all clean rebuild
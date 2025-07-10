CXX := gcc
CXXFLAGS := -Wall -Wextra
SRC_DIR := .
BUILD_DIR := build
BIN_DIR := bin

SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SOURCES))
TARGET := $(BIN_DIR)/program

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $^ -o $@ -lSDL2 -lSDL2_ttf

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -lSDL2 -lSDL2_ttf

.PHONY: clean
clean:
	rm -rf ./$(BUILD_DIR) ./$(BIN_DIR)

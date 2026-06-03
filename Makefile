# ============================================================
#  CryoCell — Battery Thermal Degradation Simulator
#  Build system
# ============================================================

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -Iinclude

SRC_DIR  := src
INC_DIR  := include
BUILD_DIR := build

# Automatically find all .cpp files in src/
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

TARGET := $(BUILD_DIR)/cryocell

# ============================================================
#  Default target — build the binary
# ============================================================
all: $(BUILD_DIR) $(TARGET)

# Link
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build successful → $(TARGET)"

# Compile each .cpp into a .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# ============================================================
#  run  — build then execute
# ============================================================
run: all
	./$(TARGET)

# ============================================================
#  clean — remove compiled artifacts
# ============================================================
clean:
	rm -rf $(BUILD_DIR)/*.o $(TARGET)
	@echo "Cleaned build artifacts."

.PHONY: all run clean

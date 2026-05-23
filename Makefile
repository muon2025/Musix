CXX = g++
CXXFLAGS = -Iinclude -Wall -O2
LDFLAGS = -lncurses -lSDL2 -lSDL2_mixer

SRC_DIR = source
OBJ_DIR = object
INC_DIR = include

# Find all .cpp files in source/ and map them to .o files in object/!
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SOURCES))
TARGET = musix

# Default rule!
all: $(TARGET)

# Link the object files into the final executable!
$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compile each .cpp file into a .o file!
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create the object directory if it doesn't exist!
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Clean up build files!
clean:
	rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
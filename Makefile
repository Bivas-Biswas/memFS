# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17

# Source files
SRC1 = main.cpp

# Target executable names
TARGET1 = memFS

# Default rule to build both targets
all: $(TARGET1)

$(TARGET1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(SRC1)

# Clean rule to remove both executables
clean:
	rm -f $(TARGET1)
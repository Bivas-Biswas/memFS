# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++17

# Source files
SRC1 = generateinputfilenames.cpp
SRC2 = generateOperationFileInput.cpp
SRC3 = benchmark.cpp

# Target executable names
TARGET1 = generateinputfilenames
TARGET2 = generateOperationFileInput
TARGET3 = benchmark

# Default rule to build both targets
# all: $(TARGET3)
all: $(TARGET1) $(TARGET2) $(TARGET3)

$(TARGET1): $(SRC1)
	$(CXX) $(CXXFLAGS) -o $(TARGET1) $(SRC1)

$(TARGET2): $(SRC2)
	$(CXX) $(CXXFLAGS) -o $(TARGET2) $(SRC2)

$(TARGET3): $(SRC3)
	$(CXX) $(CXXFLAGS) -o $(TARGET3) $(SRC3)

# Clean rule to remove both executables
clean:
	rm -f $(TARGET1) $(TARGET2) $(TARGET3)
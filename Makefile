CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude

SRC_CORE = src/sha256.cpp
OBJ_CORE = src/sha256.o

all: sha256_cli test_runner

sha256_cli: src/main.o $(OBJ_CORE)
	$(CXX) $(CXXFLAGS) -o sha256_cli src/main.o $(OBJ_CORE)

test_runner: tests/test_sha256.o $(OBJ_CORE)
	$(CXX) $(CXXFLAGS) -o test_runner tests/test_sha256.o $(OBJ_CORE)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o tests/*.o sha256_cli test_runner

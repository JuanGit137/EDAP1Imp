CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = src/counting_sort.cpp src/radix_sort.cpp src/main.cpp
OUT      = radix

all: $(OUT)

$(OUT): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)

.PHONY: all run clean

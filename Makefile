CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = src/counting_sort.cpp src/radix_sort.cpp src/main.cpp
# Los headers tambien son dependencias: si no, editar un .hpp no recompila.
HDR      = include/counting_sort.hpp include/radix_sort.hpp
OUT      = radix

all: $(OUT)

$(OUT): $(SRC) $(HDR)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(OUT)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)

.PHONY: all run clean

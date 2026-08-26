CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRC      = src/counting_sort.cpp src/radix_sort.cpp src/main.cpp
# Los headers tambien son dependencias: sin esto, editar un .hpp no
# recompilaba nada y se seguia ejecutando el binario viejo.
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

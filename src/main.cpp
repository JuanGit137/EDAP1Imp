#include "counting_sort.hpp"
#include "radix_sort.hpp"

#include <iostream>
#include <vector>

using Lista = std::vector<int>;

void imprimir(const Lista& v) {
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

void imprimir(const Matriz& M) {
    for (const auto& fila : M) {
        for (int d : fila) std::cout << d;
        std::cout << "\n";
    }
}

// Una lista de n digitos es una matriz de n x 1, asi que se ordena con el
// mismo countingSort usando col = 0. Aqui solo hay conversion, no algoritmo.
Lista ordenarLista(const Lista& v) {
    Matriz M;
    for (int x : v) M.push_back({x});

    Lista salida;
    for (const auto& fila : countingSort(M, 0)) salida.push_back(fila[0]);
    return salida;
}

int main() {
    std::cout << "=== PARTE 1: Counting Sort sobre lista ===\n\n";

    Lista A = {5, 3, 9, 0, 3, 7, 1, 9, 2};
    std::cout << "Entrada:  "; imprimir(A);
    std::cout << "Salida:   "; imprimir(ordenarLista(A));

    std::cout << "\n=== PARTE 2: Radix Sort sobre matriz 9x5 ===\n\n";

    Matriz M = {
        {3, 2, 1, 0, 9},
        {1, 0, 0, 0, 1},
        {9, 9, 9, 9, 9},
        {3, 2, 1, 0, 8},
        {0, 0, 0, 0, 0},
        {5, 4, 3, 2, 1},
        {1, 0, 0, 0, 0},
        {3, 2, 1, 0, 9},
        {7, 7, 7, 7, 6}
    };

    std::cout << "Antes:\n";
    imprimir(M);
    std::cout << "\nDespues:\n";
    imprimir(radixSort(M));

    return 0;
}

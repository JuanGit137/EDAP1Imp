#include "counting_sort.hpp"
#include "radix_sort.hpp"

#include <iostream>
#include <string>
#include <vector>

// ------------------------------------------------------------
// Utilidades de impresion y verificacion
// ------------------------------------------------------------
void imprimirLista(const std::string& etiqueta, const std::vector<int>& v) {
    std::cout << etiqueta;
    for (int x : v) std::cout << x << " ";
    std::cout << "\n";
}

void imprimirMatriz(const std::vector<std::vector<int>>& M) {
    for (const auto& fila : M) {
        for (int d : fila) std::cout << d;
        std::cout << "\n";
    }
}

bool listaOrdenada(const std::vector<int>& v) {
    for (size_t i = 0; i + 1 < v.size(); i++) {
        if (v[i] > v[i + 1]) return false;
    }
    return true;
}

bool matrizOrdenada(const std::vector<std::vector<int>>& M) {
    for (size_t f = 0; f + 1 < M.size(); f++) {
        long a = 0, b = 0;
        for (size_t c = 0; c < M[f].size(); c++) {
            a = a * 10 + M[f][c];
            b = b * 10 + M[f + 1][c];
        }
        if (a > b) return false;
    }
    return true;
}

void chequear(const std::string& nombre, bool ok) {
    std::cout << "[" << (ok ? "OK  " : "FALLA") << "] " << nombre << "\n";
}

// ------------------------------------------------------------
int main() {
    std::cout << "=== PARTE 1: Counting Sort sobre lista ===\n\n";

    std::vector<int> A = {5, 3, 9, 0, 3, 7, 1, 9, 2};
    std::vector<int> Af = sort::countingSort(A);
    imprimirLista("Entrada:  ", A);
    imprimirLista("Salida:   ", Af);
    chequear("caso general", listaOrdenada(Af));

    // Casos borde
    std::vector<int> iguales = {4, 4, 4, 4, 4, 4, 4, 4, 4};
    chequear("todos iguales", listaOrdenada(sort::countingSort(iguales)));

    std::vector<int> descendente = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    chequear("orden inverso", listaOrdenada(sort::countingSort(descendente)));

    std::vector<int> extremos = {0, 9, 0, 9, 0, 9, 0, 9, 0};
    chequear("solo 0 y 9", listaOrdenada(sort::countingSort(extremos)));

    std::vector<int> vacia = {};
    chequear("lista vacia", listaOrdenada(sort::countingSort(vacia)));

    std::cout << "\n=== PARTE 2: Radix Sort sobre matriz 9x5 ===\n\n";

    std::vector<std::vector<int>> M = {
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
    imprimirMatriz(M);

    std::vector<std::vector<int>> R = sort::radixSort(M);

    std::cout << "\nDespues:\n";
    imprimirMatriz(R);

    std::cout << "\n";
    chequear("matriz ordenada", matrizOrdenada(R));

    // Verificar una pasada aislada de countingSort por columna
    std::vector<std::vector<int>> unaPasada = sort::countingSort(M, 4);
    bool colOk = true;
    for (size_t f = 0; f + 1 < unaPasada.size(); f++) {
        if (unaPasada[f][4] > unaPasada[f + 1][4]) colOk = false;
    }
    chequear("una pasada por columna 4", colOk);

    // Verificar ESTABILIDAD: las tres filas 32109 / 32108 / 32109 deben
    // conservar su orden relativo tras ordenar por una columna donde empatan.
    // Columna 0: las filas 0, 3 y 7 tienen todas el digito 3, y son las
    // unicas con ese digito. Antes van 1 fila con 0 y 2 filas con 1,
    // asi que deben quedar en las posiciones 3, 4 y 5, en ese mismo orden.
    std::vector<std::vector<int>> estable = sort::countingSort(M, 0);
    bool estOk = (estable[3] == M[0] && estable[4] == M[3] && estable[5] == M[7]);
    chequear("estabilidad (empates conservan orden)", estOk);

    return 0;
}

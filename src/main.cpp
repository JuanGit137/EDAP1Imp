#include "counting_sort.hpp"
#include "radix_sort.hpp"

#include <iostream>
#include <string>
#include <vector>

// ---------- impresion ----------

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

// ---------- verificacion ----------

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

std::vector<int> histograma(const std::vector<int>& v) {
    std::vector<int> h(10, 0);
    for (int x : v) h[x]++;
    return h;
}

// Verificar solo el ORDEN no basta: {0,0,0,...} tambien esta ordenado, y el
// vector de salida se inicializa en ceros, asi que un countingSort que no
// llenara nada pasaria el test. Hay que exigir tambien mismo contenido.
bool ordenaBien(const std::vector<int>& entrada, const std::vector<int>& salida) {
    return listaOrdenada(salida)
        && entrada.size() == salida.size()
        && histograma(entrada) == histograma(salida);
}

// Mismo agujero en la Parte 2. Es un VERIFICADOR, no parte del algoritmo:
// aqui el O(n^2) da igual.
bool mismasFilas(const std::vector<std::vector<int>>& M,
                 const std::vector<std::vector<int>>& Mf) {
    if (M.size() != Mf.size()) return false;
    std::vector<bool> usada(M.size(), false);
    for (const auto& fila : Mf) {
        bool encontrada = false;
        for (size_t i = 0; i < M.size() && !encontrada; i++) {
            if (!usada[i] && M[i] == fila) {
                usada[i] = true;
                encontrada = true;
            }
        }
        if (!encontrada) return false;
    }
    return true;
}

// ---------- Parte 1 con la funcion de la Parte 2 ----------
//
// Una lista de n digitos es una matriz de n x 1, asi que se ordena con el
// mismo countingSort usando col = 0. Aqui solo hay conversion, no algoritmo.

std::vector<std::vector<int>> comoMatriz(const std::vector<int>& v) {
    std::vector<std::vector<int>> M;
    for (int x : v) M.push_back({x});
    return M;
}

std::vector<int> comoLista(const std::vector<std::vector<int>>& M) {
    std::vector<int> v;
    for (const auto& fila : M) v.push_back(fila[0]);
    return v;
}

std::vector<int> ordenarLista(const std::vector<int>& v) {
    return comoLista(sort::countingSort(comoMatriz(v), 0));
}

// Se cuenta para devolverlo como codigo de salida al final de main().
int fallas = 0;

void chequear(const std::string& nombre, bool ok) {
    std::cout << "[" << (ok ? "OK  " : "FALLA") << "] " << nombre << "\n";
    if (!ok) fallas++;
}

int main() {
    std::cout << "=== PARTE 1: Counting Sort sobre lista ===\n\n";

    std::vector<int> A = {5, 3, 9, 0, 3, 7, 1, 9, 2};
    std::vector<int> Af = ordenarLista(A);
    imprimirLista("Entrada:  ", A);
    imprimirLista("Salida:   ", Af);
    chequear("caso general", ordenaBien(A, Af));

    // Casos borde
    std::vector<int> iguales = {4, 4, 4, 4, 4, 4, 4, 4, 4};
    chequear("todos iguales", ordenaBien(iguales, ordenarLista(iguales)));

    std::vector<int> descendente = {9, 8, 7, 6, 5, 4, 3, 2, 1};
    chequear("orden inverso", ordenaBien(descendente, ordenarLista(descendente)));

    std::vector<int> extremos = {0, 9, 0, 9, 0, 9, 0, 9, 0};
    chequear("solo 0 y 9", ordenaBien(extremos, ordenarLista(extremos)));

    std::vector<int> vacia = {};
    chequear("lista vacia", ordenaBien(vacia, ordenarLista(vacia)));

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
    chequear("matriz ordenada", matrizOrdenada(R) && mismasFilas(M, R));

    // Una pasada aislada de countingSort por columna
    std::vector<std::vector<int>> unaPasada = sort::countingSort(M, 4);
    bool colOk = mismasFilas(M, unaPasada);
    for (size_t f = 0; f + 1 < unaPasada.size(); f++) {
        if (unaPasada[f][4] > unaPasada[f + 1][4]) colOk = false;
    }
    chequear("una pasada por columna 4", colOk);

    // ESTABILIDAD. Las filas que empatan tienen que ser DISTINGUIBLES entre
    // si, o el test no sirve: si dos son identicas, intercambiarlas no cambia
    // nada y un algoritmo inestable pasaria igual. Estas tres empatan en la
    // columna 0 pero llevan marca distinta en el ultimo digito.
    std::vector<std::vector<int>> conEmpates = {
        {4, 0, 0, 0, 1},   // marca 1
        {2, 9, 9, 9, 9},
        {4, 0, 0, 0, 2},   // marca 2
        {1, 1, 1, 1, 1},
        {4, 0, 0, 0, 3}    // marca 3
    };

    // Ordenado por la columna 0 quedan: el 1, el 2, y las tres del 4 en su
    // orden de aparicion -> posiciones 2, 3 y 4.
    std::vector<std::vector<int>> estable = sort::countingSort(conEmpates, 0);
    bool estOk = (estable[2] == conEmpates[0] &&
                  estable[3] == conEmpates[2] &&
                  estable[4] == conEmpates[4]);
    chequear("estabilidad (empates conservan orden)", estOk);

    std::cout << "\n" << (fallas == 0
                          ? "Todas las pruebas pasaron.\n"
                          : "PRUEBAS FALLIDAS: " + std::to_string(fallas) + "\n");
    return fallas == 0 ? 0 : 1;
}

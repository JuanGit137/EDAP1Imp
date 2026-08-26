#include "counting_sort.hpp"
#include <deque>

namespace sort {

// Rango de valores posibles de un digito: 0..9  ->  10 casilleros.
// Es CONSTANTE, y por eso el algoritmo se mantiene en O(n).
static const int K = 10;

// ------------------------------------------------------------
// PARTE 1 — Counting Sort sobre una lista de digitos
//
// Traduccion directa del pseudocodigo:
//
//   A[0...8]
//   Af[0...8]
//   for i <- 0...9
//       Bi <- zeros                 (se crean los casilleros)
//   end for
//   for i <- 0...8
//       B[A[i]].append(i)           (guarda la POSICION i en el casillero A[i])
//   end for
//   p <- 0
//   i <- 0
//   while i <= 9 do
//       if Bi no esta vacio
//           Af[p] <- sacar de Bi
//           p <- p + 1
//       else
//           i <- i + 1              (casillero vacio -> pasar al siguiente)
//       end if
//   end while
//   return Af
// ------------------------------------------------------------
std::vector<int> countingSort(const std::vector<int>& A) {
    int n = static_cast<int>(A.size());
    std::vector<int> Af(n);

    // deque, no vector: pop_front() es O(1).
    // Con vector habria que usar erase(begin()), que es O(tamano del casillero)
    // y en el peor caso (todos los digitos iguales) degenera a O(n^2).
    std::deque<int> B[K];

    // for i <- 0...9 : Bi <- zeros
    for (int i = 0; i < K; i++) {
        B[i].clear();
    }

    // for i <- 0...n-1 : B[A[i]].append(i)
    // Se guarda el INDICE, no el valor. En la Parte 2 esto es lo que permite
    // saber que fila mover.
    for (int i = 0; i < n; i++) {
        B[A[i]].push_back(i);
    }

    int p = 0;
    int i = 0;
    while (i < K) {
        if (!B[i].empty()) {
            // "Sacar del casillero" en C++ son DOS operaciones:
            //   front()     -> mira el primero, no lo saca
            //   pop_front() -> lo elimina, no devuelve nada
            int posicion = B[i].front();
            B[i].pop_front();

            Af[p] = A[posicion];
            p = p + 1;
        } else {
            i = i + 1;
        }
    }

    return Af;
}

// ------------------------------------------------------------
// PARTE 2 — El MISMO algoritmo, sobre una matriz
//
// Solo cambian tres cosas respecto de la Parte 1:
//   (1) el casillero lo elige M[i][col] en vez de A[i]
//   (2) el resultado es una matriz en vez de una lista
//   (3) al sacar, se copia la FILA COMPLETA
// ------------------------------------------------------------
std::vector<std::vector<int>> countingSort(const std::vector<std::vector<int>>& M,
                                           int col) {
    int n = static_cast<int>(M.size());
    std::vector<std::vector<int>> Mf(n);

    std::deque<int> B[K];

    for (int i = 0; i < K; i++) {
        B[i].clear();
    }

    // (1) el digito de la columna 'col' decide el casillero.
    // push_back en orden de aparicion + pop_front al sacar (FIFO)
    // = ESTABILIDAD, que es lo que hace funcionar al Radix Sort.
    for (int i = 0; i < n; i++) {
        B[M[i][col]].push_back(i);
    }

    int p = 0;
    int i = 0;
    while (i < K) {
        if (!B[i].empty()) {
            int fila = B[i].front();
            B[i].pop_front();

            // (3) se mueve la fila entera.
            // Copiar un vector de 5 elementos es O(5) = constante.
            Mf[p] = M[fila];
            p = p + 1;
        } else {
            i = i + 1;
        }
    }

    return Mf;
}

} // namespace sort

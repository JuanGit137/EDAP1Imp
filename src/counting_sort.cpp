#include "counting_sort.hpp"
#include <queue>

// Rango de un digito: 0..9 -> 10 casilleros. Es CONSTANTE, y por eso el
// algoritmo se mantiene en O(n).
static const int K = 10;

Matriz countingSort(const Matriz& M, int col) {
    int n = static_cast<int>(M.size());
    Matriz Mf(n);

    // Cada casillero es una cola FIFO. Sacar del frente es O(1); con vector
    // habria que usar erase(begin()) y en el peor caso (todos los digitos
    // iguales, todo en un casillero) degeneraria a O(n^2).
    std::queue<int> B[K];

    // Se guarda el INDICE de la fila, no el digito: eso es lo que permite
    // mover la fila entera al sacarla. Y entrar por atras para salir por
    // delante es lo que hace ESTABLE al algoritmo, sin lo cual Radix falla.
    for (int i = 0; i < n; i++) {
        B[M[i][col]].push(i);
    }

    int p = 0;
    int i = 0;
    while (i < K) {
        if (!B[i].empty()) {
            int fila = B[i].front();
            B[i].pop();

            Mf[p] = M[fila];   // copiar una fila es O(COLS) = constante
            p = p + 1;
        } else {
            i = i + 1;         // casillero vacio -> pasar al siguiente
        }
    }

    return Mf;
}

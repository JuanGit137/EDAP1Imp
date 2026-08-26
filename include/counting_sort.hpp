#ifndef COUNTING_SORT_HPP
#define COUNTING_SORT_HPP

#include <vector>

namespace sort {

// PARTE 1
// Ordena ascendentemente una lista de digitos (0-9).
// Recibe la lista y devuelve una lista nueva ordenada.
// Complejidad: O(n + k), con k = 10 constante  ->  O(n)
std::vector<int> countingSort(const std::vector<int>& A);

// PARTE 2
// Ordena las FILAS de una matriz segun el digito de la columna 'col'.
// Mueve la fila completa, no solo el digito.
// Es ESTABLE: filas con el mismo digito conservan su orden relativo.
// Complejidad: O(n * COLS + k)  ->  O(n), con COLS y k constantes
std::vector<std::vector<int>> countingSort(const std::vector<std::vector<int>>& M,
                                           int col);

} // namespace sort

#endif

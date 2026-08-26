#ifndef COUNTING_SORT_HPP
#define COUNTING_SORT_HPP

#include <vector>

namespace sort {

// Ordena las FILAS de M segun el digito de la columna 'col', moviendo la
// fila completa. Es ESTABLE: las filas que empatan conservan su orden
// relativo, requisito sin el cual Radix Sort no funciona.
//
// Cubre las dos partes del enunciado: una lista de n digitos es una matriz
// de n x 1, y se ordena con col = 0.
//
// Complejidad: O(n * COLS + k) -> O(n), con COLS y k constantes.
std::vector<std::vector<int>> countingSort(const std::vector<std::vector<int>>& M,
                                           int col);

} // namespace sort

#endif

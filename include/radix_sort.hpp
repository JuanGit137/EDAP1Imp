#ifndef RADIX_SORT_HPP
#define RADIX_SORT_HPP

#include <vector>

namespace sort {

// Ordena una matriz de n filas x d columnas, donde cada fila es un numero
// de d digitos. Recorre las columnas de la ULTIMA a la PRIMERA y aplica
// countingSort en cada pasada.
//
// Complejidad: O(d * (n + k)) = O(n), con d y k constantes.
std::vector<std::vector<int>> radixSort(const std::vector<std::vector<int>>& M);

} // namespace sort

#endif

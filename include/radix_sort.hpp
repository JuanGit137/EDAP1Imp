#ifndef RADIX_SORT_HPP
#define RADIX_SORT_HPP

#include "counting_sort.hpp"   // Matriz y countingSort

// Ordena una matriz de n filas x d columnas, donde cada fila es un numero
// de d digitos. Recorre las columnas de la ULTIMA a la PRIMERA y aplica
// countingSort en cada pasada.
//
// Complejidad: O(d * (n + k)) = O(n), con d y k constantes.
Matriz radixSort(const Matriz& M);

#endif

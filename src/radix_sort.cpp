#include "radix_sort.hpp"

// Toda la inteligencia esta en countingSort; esto solo decide EN QUE ORDEN
// se aplican las pasadas. De la ultima columna a la primera (LSD): el digito
// menos significativo se ordena primero para que el mas significativo mande
// al final, y la estabilidad conserva el trabajo previo como desempate.
Matriz radixSort(const Matriz& M) {
    if (M.empty()) {
        return M;
    }

    int cols = static_cast<int>(M[0].size());
    Matriz resultado = M;

    for (int columna = cols - 1; columna >= 0; columna--) {
        resultado = countingSort(resultado, columna);
    }

    return resultado;
}

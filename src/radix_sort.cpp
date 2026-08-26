#include "radix_sort.hpp"
#include "counting_sort.hpp"

namespace sort {

// Toda la inteligencia esta en countingSort; esto solo decide EN QUE ORDEN
// se aplican las pasadas. De la ultima columna a la primera (LSD): el digito
// menos significativo se ordena primero para que el mas significativo mande
// al final, y la estabilidad conserva el trabajo previo como desempate.
std::vector<std::vector<int>> radixSort(const std::vector<std::vector<int>>& M) {
    if (M.empty()) {
        return M;
    }

    int cols = static_cast<int>(M[0].size());
    std::vector<std::vector<int>> resultado = M;

    for (int columna = cols - 1; columna >= 0; columna--) {
        resultado = countingSort(resultado, columna);
    }

    return resultado;
}

} // namespace sort

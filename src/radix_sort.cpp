#include "radix_sort.hpp"
#include "counting_sort.hpp"

namespace sort {

// ------------------------------------------------------------
// Radix Sort — LSD (Least Significant Digit)
//
// Toda la inteligencia esta en countingSort. Radix es solo el bucle
// que decide EN QUE ORDEN se aplican las pasadas.
//
// Se va de la ultima columna a la primera (4 -> 3 -> 2 -> 1 -> 0)
// porque el digito menos significativo debe ordenarse primero: cada
// pasada posterior "manda" mas, y la estabilidad conserva el trabajo
// de las pasadas anteriores como criterio de desempate.
// ------------------------------------------------------------
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

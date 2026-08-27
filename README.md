# Counting Sort + Radix Sort

Implementación en C++ de Counting Sort y Radix Sort en O(n), a partir de un
pseudocódigo diseñado a mano.

```bash
make clean & make run
```

---

## 1. El problema

**Parte 1.** Dada una lista de 9 dígitos, ordenarla ascendentemente. En O(n).

**Parte 2.** Dada una matriz de 9 × 5 donde cada fila es un número de 5 dígitos,
ordenar las filas reutilizando el Counting Sort anterior: una pasada por
columna, de la última a la primera (4 → 3 → 2 → 1 → 0). En cada pasada se mueve
la **fila completa** según el dígito de esa columna. También en O(n).

**Restricción.** Ninguna comparación entre elementos: nada de `std::sort`,
QuickSort, BubbleSort ni SelectionSort.

Las dos partes se resuelven con **una sola función**, `countingSort`, la que
opera sobre matrices: una lista de n dígitos es el caso particular de una matriz
de n × 1, y se ordena con `col = 0`.

---

## 2. Los archivos

| Archivo | Qué hace |
|---|---|
| `include/counting_sort.hpp` | el alias `Matriz` y la declaración de `countingSort` |
| `include/radix_sort.hpp` | la declaración de `radixSort` |
| `src/counting_sort.cpp` | **el algoritmo** — una pasada por una columna |
| `src/radix_sort.cpp` | el bucle que llama a `countingSort` una vez por columna |
| `src/main.cpp` | demostración: ordena una lista y una matriz, e imprime |
| `Makefile` | compila con `-std=c++17 -Wall -Wextra` |

`Matriz` es solo un nombre corto para `std::vector<std::vector<int>>`; no es un
tipo nuevo.

**Toda la inteligencia está en `counting_sort.cpp`.** `radix_sort.cpp` no
reimplementa nada: solo decide *en qué orden* se aplican las pasadas.

```cpp
for (int columna = cols - 1; columna >= 0; columna--) {
    resultado = countingSort(resultado, columna);
}
```

Fíjate en `resultado = countingSort(resultado, ...)`: cada pasada trabaja sobre
la salida de la anterior. Se va de derecha a izquierda (LSD) para que el dígito
menos significativo se ordene primero y el más significativo mande al final.

---

## 3. Cómo funciona Counting Sort

### La idea

No compara elementos: los reparte.

Imagina 10 casilleros numerados del 0 al 9. Tiras cada número al casillero que
lleva su mismo dígito. Al terminar no comparaste nada — pero los casilleros ya
están en orden porque *tú* los numeraste en orden. Solo queda recorrerlos del 0
al 9 y vaciarlos.

**El orden no se descubre comparando, se impone por la estructura.** Por eso no
hay ningún `if (a > b)` en el algoritmo.

Esto solo funciona si el rango de valores es chico y conocido. Aquí lo es:
dígitos de 0 a 9, siempre 10 casilleros, sin importar cuántos números haya.

### El código

```cpp
Matriz countingSort(const Matriz& M, int col) {
    int n = static_cast<int>(M.size());
    Matriz Mf(n);                    // la salida, n filas ya reservadas

    std::queue<int> B[K];            // los 10 casilleros, K = 10
```

**Fase 1 — repartir.** El dígito de la columna elige el casillero:

```cpp
    for (int i = 0; i < n; i++) {
        B[M[i][col]].push(i);
    }
```

Léela de dentro hacia fuera: `M[i][col]` es el dígito de la fila `i`, `B[...]`
es la cola de ese dígito, y `.push(i)` mete **el número de fila** al final.

Se guarda el índice `i`, no el dígito. El dígito ya lo sabes — es el número del
casillero. Lo que necesitas recordar es *qué fila* mover después.

**Fase 2 — vaciar.** Se recorren los casilleros del 0 al 9:

```cpp
    int p = 0;                       // dónde escribir en la salida
    int i = 0;                       // qué casillero estoy vaciando
    while (i < K) {
        if (!B[i].empty()) {
            int fila = B[i].front(); // mirar el primero
            B[i].pop();              // sacarlo (son dos operaciones)

            Mf[p] = M[fila];         // mover la FILA COMPLETA
            p = p + 1;
        } else {
            i = i + 1;               // casillero vacío -> siguiente
        }
    }

    return Mf;
}
```

Dos contadores independientes: `p` avanza cuando sacas algo, `i` cuando un
casillero se acaba. El `else` es lo único que mueve `i`, así que te quedas en el
mismo casillero mientras quede algo dentro.

### Por qué es estable — y por qué eso importa

Un algoritmo es **estable** si los elementos que empatan conservan su orden
relativo. Este lo es porque las filas entran con `push` (por atrás) y salen con
`front` + `pop` (por delante): FIFO, el primero que entra es el primero que sale.

Sin estabilidad el Radix no funciona. La pasada por la columna 3 destruiría el
trabajo de la pasada por la columna 4. Con estabilidad, en cambio, las filas que
empatan en la columna 3 mantienen el orden que les dejó la columna 4 — el
trabajo previo sobrevive como **criterio de desempate**.

Por eso se usa `std::queue` y no `std::vector`: `vector` no sabe sacar del
frente, habría que usar `erase(begin())` —que corre todos los elementos
restantes— y en el peor caso (todos los dígitos iguales, todo en un casillero)
degeneraría a O(n²).

### Complejidad

| Bloque | Costo |
|---|---|
| Crear los 10 casilleros | O(k) |
| Repartir las n filas | O(n) |
| Vaciar los casilleros | O(n·d + k) |
| **Counting Sort (una pasada)** | **O(n·d + k)** |
| **Radix Sort** (d pasadas) | **O(d · (n·d + k))** |

Con `k = 10` dígitos y `d = 5` columnas —ambos constantes, fijados por el
enunciado— todo se reduce a **O(n)**.

El matiz que conviene tener claro para la defensa: esto es O(n) *porque* d y k
son constantes. Si el rango k creciera con n, el término O(k) dejaría de ser
despreciable y el análisis cambiaría.

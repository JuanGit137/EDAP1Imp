# Counting Sort + Radix Sort — Prueba EDA

Implementación en C++ de Counting Sort y Radix Sort en O(n), a partir de un
pseudocódigo diseñado a mano. Este documento explica el razonamiento completo:
qué hace cada parte, por qué se tomó cada decisión, y qué hubo que arreglar al
pasar de pseudocódigo a código real.

---

## 1. El problema

**Parte 1 — Counting Sort.** Dada una lista de 9 dígitos, ordenarla
ascendentemente. La función recibe la lista y devuelve la lista ordenada.
Complejidad exigida: O(n).

**Parte 2 — Radix Sort.** Dada una matriz de 9 × 5 donde cada fila representa un
número de 5 dígitos, ordenar las filas reutilizando el Counting Sort anterior,
columna por columna, desde la última hasta la primera (4 → 3 → 2 → 1 → 0). En
cada pasada se ordenan las **filas completas** según el dígito de esa columna.
Complejidad exigida: O(n).

**Restricción.** Nada de comparaciones entre elementos: ni `std::sort`, ni
QuickSort, ni BubbleSort, ni SelectionSort. Ningún ciclo que supere la
linealidad.

> **Nota de implementación.** Las dos partes se resuelven con **una sola**
> función `countingSort`, la que opera sobre matrices: una lista de dígitos es
> el caso particular de una matriz de una columna. El detalle está en la
> sección 6.

---

## 2. La idea: casilleros

Counting Sort no compara elementos. Los reparte.

Imagina 10 casilleros numerados del 0 al 9. Vas tomando cada número de la lista
y lo tiras al casillero que lleva su mismo número. Cuando terminas, no
comparaste nada — pero los casilleros ya están en orden porque *tú* los
numeraste en orden. Solo queda recorrerlos del 0 al 9 y vaciarlos uno por uno.

Ahí está el truco: **el orden no se descubre comparando, se impone por la
estructura**. Por eso no hay `if (a > b)` en ninguna parte del algoritmo.

Esto solo funciona si el rango de valores es chico y conocido. Aquí lo es:
dígitos de 0 a 9, siempre 10 casilleros, sin importar cuántos números haya. Eso
es lo que mantiene el algoritmo en O(n).

---

## 3. El pseudocódigo original

```
A [0...8]
Af [0...8]

for i <- 0...9
    Bi <- zeros                     // se crean los casilleros
end for

for i <- 0...8
    B[A[i]].append(i)               // se agrega la posición i de A[i]
end for                             // en la posición A[i] de B

p <- 0
i <- 0
while i <= 9 do
    if Bi != zeros                  // si no está vacío
        Af[p] <- sacar de Bi        // se va agregando lo que está
        p <- p + 1                  // en el casillero no vacío a Af[p]
    else
        i <- i + 1                  // ya está vacío, pasar al siguiente
    end if
end while

Return Af
```

La estructura del `while` es correcta: **sacar mientras haya, avanzar cuando se
acabe**. Eso es exactamente un Counting Sort con casilleros.

Un detalle del diseño que vale la pena destacar: en el segundo `for` se guarda
la **posición** `i`, no el valor `A[i]`. En la Parte 1 da lo mismo — pero en la
Parte 2 es justo lo que hace que todo salga gratis, porque el casillero pasa a
decirte *qué fila* mover.

### Los dos ajustes que hubo que hacer

**(a) Los casilleros van de 0 a 9, no de 0 a 8.**

El índice del casillero es el *dígito*, no la posición del arreglo. Como los
dígitos llegan hasta 9, hacen falta 10 casilleros. Con `0...8` no habría dónde
poner un 9.

**(b) Falta sacar el elemento del casillero.**

`Af[p] <- Bi` copia lo que hay en el casillero, pero no lo elimina. Entonces
`Bi != zeros` sigue siendo verdadero para siempre, el `else` nunca se ejecuta,
`i` nunca avanza y el `while` se cuelga.

En pseudocódigo "sacar" es una sola idea. En C++ son dos operaciones separadas,
y esa es la traducción que hay que aprender:

```cpp
int posicion = B[i].front();   // 1. MIRAR el primero — no lo saca
B[i].pop();                    // 2. SACARLO — no devuelve nada
```

Ninguna función hace las dos cosas a la vez. Van siempre en pareja y en ese
orden: si haces `pop()` primero, perdiste el dato.

### Tabla de traducción

| Pseudocódigo               | C++                            |
|----------------------------|--------------------------------|
| `B[i].append(x)`           | `B[i].push(x)`                 |
| `Bi != zeros` (¿tiene algo?)| `!B[i].empty()`               |
| sacar el primero           | `B[i].front()` + `B[i].pop()`  |
| `Bi <- zeros` (crear vacío)| nada: la cola ya nace vacía     |

---

## 4. Por qué `queue` y no `vector`

Un casillero **es una cola**: se entra por atrás y se sale por delante. El
pseudocódigo pide exactamente tres operaciones sobre él —`append`, «¿tiene
algo?», «sacar»— y eso es la definición de una cola FIFO.

```cpp
std::queue<int> B[10];
```

`std::queue` expone solo `push`, `front`, `pop` y `empty`. Nada más. No hay
sufijos `_front` / `_back` que leer en cada línea, y no existe ninguna operación
capaz de romper el FIFO.

### El problema de `vector`

`vector` tiene `push_back` (el `append` del pseudocódigo) pero **no sabe sacar
del frente**. Habría que escribir `B[i].erase(B[i].begin())`, que corre todos
los elementos restantes una posición a la izquierda: O(tamaño del casillero) por
extracción.

En el peor caso —todos los dígitos iguales, así que los n elementos caen en un
mismo casillero— eso da n + (n−1) + (n−2) + … = **O(n²)**, y se pierde la
linealidad que exige el enunciado. Medido, vaciando un solo casillero:

```
     n |    queue |   vector + erase
-------+----------+------------------
 10000 |    0.04  |        2.75 ms
 20000 |    0.05  |       13.38 ms    ← ×4.9
 40000 |    0.10  |       50.37 ms    ← ×3.8
 80000 |    0.15  |      214.75 ms    ← ×4.3
```

Al doblar n, `vector` cuadruplica el tiempo: la firma de O(n²). `queue` lo
dobla: lineal.

### Dónde entra `deque`

`std::queue` no es una estructura nueva: es una fachada sobre `std::deque`
(*double-ended queue*), que es su contenedor por defecto.

```cpp
template<class T, class Container = std::deque<T>> class queue;
```

`deque` no guarda todo en un bloque contiguo como `vector`, sino en varios
bloques con un índice de punteros, de modo que el inicio «flota» y sacar del
frente es mover una marca — O(1), sin tocar ningún elemento.

Ventaja extra: **el error ni siquiera se puede escribir**. Intentar montar la
cola sobre `vector` no compila:

```cpp
std::queue<int, std::vector<int>> B;   // error: 'std::vector<int>' has no
                                       // member named 'pop_front'
```

Con `queue`, cada extracción es O(1) y el total del `while` es:

```
n extracciones + 10 avances de i  =  O(n + k)
```

---

## 5. Por qué las funciones devuelven en vez de usar `void`

En C++ **no se puede retornar un arreglo crudo**. Esto no compila:

```cpp
int[] countingSort(int A[], int n) { ... }   // error de sintaxis
```

Y esto compila pero está roto:

```cpp
int* countingSort(int A[], int n) {
    int Af[9];        // vive solo dentro de la función
    ...
    return Af;        // puntero a memoria que ya murió
}
```

Los arreglos crudos obligan a pasar el arreglo de salida como parámetro, con la
función declarada `void`. Como el enunciado pide *devolver* la lista ordenada,
se usa `std::vector`, que sí se puede retornar porque se copia/mueve completo.

Ventajas adicionales:

- **Mover una fila completa es una línea**: `Mf[p] = M[fila];` en vez de un
  `for` que copie dígito por dígito.
- **Una sola función cubre las dos partes.** Al trabajar con `vector`, la
  versión de matrices sirve también para listas sin escribir nada nuevo (ver
  sección 6).

Los parámetros van como `const vector<...>&` para evitar copiar la entrada al
entrar a la función.

---

## 6. Una sola función para las dos partes

Hay **una única implementación** de Counting Sort en el proyecto: la que trabaja
sobre matrices. La Parte 1 no necesita código propio, porque una lista de n
dígitos no es más que una matriz de n filas × 1 columna:

```cpp
// ordenar {5,3,9,…} equivale a ordenar {{5},{3},{9},…} por la columna 0
countingSort(comoMatriz(A), 0);
```

`comoMatriz()` y `comoLista()` viven en `main.cpp` y no contienen algoritmo
alguno: solo convierten de ida y vuelta.

### Qué cambió respecto del pseudocódigo original

El pseudocódigo estaba escrito para una lista. Pasar a matriz cambia **tres**
cosas:

| # | Pseudocódigo (lista)     | Implementación (matriz)                |
|---|--------------------------|----------------------------------------|
| 1 | `B[A[i]].push(i)`        | `B[M[i][col]].push(i)` — el dígito de la columna elige el casillero |
| 2 | `vector<int> Af`         | `vector<vector<int>> Mf`               |
| 3 | `Af[p] = A[posicion]`    | `Mf[p] = M[fila]` — mueve la fila completa |

El resto —crear casilleros, el `while`, el `if/else`, el contador `p`— es
idéntico. Ese es justamente el motivo por el que basta con una sola versión: la
de matrices ya *es* la de listas, con una columna.

Copiar una fila es O(5). Como 5 es constante, la complejidad no cambia.

---

## 7. La estabilidad es lo que hace funcionar al Radix

Esto es lo más importante del diseño, y es fácil que pase desapercibido.

Un algoritmo de ordenamiento es **estable** si los elementos que empatan
conservan su orden relativo original.

El Counting Sort de aquí es estable porque:

- las filas entran al casillero con `push`, **en orden de aparición**
- y salen por el frente con `front` + `pop`, o sea **FIFO**: el primero que entró es el primero
  que sale

¿Por qué importa? Porque el Radix ordena por la columna 4 y *después* por la
columna 3. Si la segunda pasada no fuera estable, destruiría por completo el
trabajo de la primera y el resultado sería basura.

Con estabilidad, en cambio, cada pasada nueva manda más que la anterior, y el
trabajo previo sobrevive como **criterio de desempate**. Por eso se va de
derecha a izquierda: el dígito menos significativo se ordena primero, y el más
significativo —el que más manda— se ordena al final.

Ejemplo concreto. Al ordenar por la columna 0, las tres filas `32109`, `32108` y
`32109` empatan (todas empiezan en 3). Como Counting Sort es estable, quedan en
el mismo orden relativo en que estaban — que es el orden que las pasadas por las
columnas 4, 3, 2 y 1 ya habían dejado correcto.

Para *verificar* la estabilidad hace falta un cuidado extra: las filas que
empatan tienen que ser **distinguibles entre sí**. En la matriz de ejemplo dos de
esas tres filas son idénticas (`32109` está repetida), así que un algoritmo
inestable que las intercambiara produciría exactamente la misma salida y el test
no vería nada. Por eso el test `estabilidad (empates conservan orden)` usa una
matriz aparte, donde las tres filas que empatan llevan una marca distinta en el
último dígito (1, 2 y 3): cualquier alteración del orden relativo se vuelve
visible.

---

## 8. Radix Sort

Toda la inteligencia está en `countingSort`. Radix es solo el bucle que decide
**en qué orden** se aplican las pasadas:

```cpp
for (int columna = cols - 1; columna >= 0; columna--) {
    resultado = countingSort(resultado, columna);
}
```

Cinco líneas. Es lo que se buscaba.

---

## 9. Complejidad

**Counting Sort (una pasada):**

| Bloque                    | Costo    |
|---------------------------|----------|
| Crear los 10 casilleros   | O(k)     |
| Repartir las n filas      | O(n)     |
| Vaciar los casilleros     | O(n·d + k) |
| **Total**                 | **O(n·d + k)** |

Cada extracción copia una fila de d elementos. Como `d = 5` y `k = 10` son
constantes y no dependen de n: **O(n)**.

En la Parte 1, d = 1 (una columna), y la fórmula se reduce al caso clásico
O(n + k) → **O(n)**.

**Radix Sort:** d pasadas de Counting Sort:

```
O(d · (n + k))  con d = 5 y k = 10 constantes  →  O(n)
```

**Cero comparaciones entre elementos.** No hay ningún `if (a > b)` en todo el
algoritmo. El orden sale de la numeración de los casilleros.

Nota sobre el "truco" de la constante: esto es O(n) *porque* d y k están fijos
por el enunciado (5 columnas, dígitos 0–9). Si el rango k creciera con n, el
término O(k) dejaría de ser constante y el análisis cambiaría. Vale la pena
tenerlo claro por si lo preguntan en la defensa.

---

## 10. Estructura del proyecto

```
eda-radix/
├── .gitignore
├── Makefile
├── README.md
├── include/
│   ├── counting_sort.hpp
│   └── radix_sort.hpp
└── src/
    ├── counting_sort.cpp     # la unica version: sobre matrices
    ├── radix_sort.cpp        # el bucle de 5 líneas
    └── main.cpp              # pruebas y verificación
```

### Compilar y ejecutar

```bash
make        # compila con -std=c++17 -Wall -Wextra
make run    # compila y ejecuta
make clean  # borra el binario
```

O directo, sin Make:

```bash
g++ -std=c++17 -Iinclude src/*.cpp -o radix && ./radix
```

### Salida esperada

```
=== PARTE 1: Counting Sort sobre lista ===

Entrada:  5 3 9 0 3 7 1 9 2
Salida:   0 1 2 3 3 5 7 9 9
[OK  ] caso general
[OK  ] todos iguales
[OK  ] orden inverso
[OK  ] solo 0 y 9
[OK  ] lista vacia

=== PARTE 2: Radix Sort sobre matriz 9x5 ===

Antes:
32109
10001
99999
32108
00000
54321
10000
32109
77776

Despues:
00000
10000
10001
32108
32109
32109
54321
77776
99999

[OK  ] matriz ordenada
[OK  ] una pasada por columna 4
[OK  ] estabilidad (empates conservan orden)

Todas las pruebas pasaron.
```

El programa devuelve **código de salida 0 si todo pasa y 1 si algo falla**, así
que `make run` sirve para automatizar:

```bash
make run && echo "verde"
```

### Qué verifica cada prueba

No basta con comprobar que la salida esté ordenada: el vector de salida se crea
inicializado en ceros, y `{0,0,0,…}` también está ordenado — un algoritmo que no
llenara nada pasaría el test. Por eso cada prueba comprueba **dos** cosas:

| Comprobación | Cómo |
|---|---|
| La salida está ordenada | recorrido lineal comparando vecinos |
| La salida tiene los mismos elementos que la entrada | histograma de los 10 dígitos (Parte 1) / emparejamiento de filas (Parte 2) |

Compilado y verificado con `g++ -Wall -Wextra`, sin warnings.

---

## 11. Resumen de las decisiones

| Decisión | Razón |
|---|---|
| 10 casilleros, no 9 | el índice es el dígito (0–9), no la posición |
| `front()` + `pop()` | en C++ "sacar" son dos operaciones |
| `queue`, no `vector` | sacar del frente en O(1); con `vector` degenera a O(n²) |
| `queue`, no `deque` a secas | expone solo las 3 operaciones del pseudocódigo; el FIFO no se puede romper por accidente |
| Guardar el índice, no el valor | permite mover la fila completa en la Parte 2 |
| `push` + `pop` (FIFO) | garantiza estabilidad, sin la cual el Radix falla |
| Retornar `vector` | no se puede retornar un arreglo crudo en C++ |
| Una sola `countingSort`, la de matrices | la Parte 1 es el caso de 1 columna; dos implementaciones serían el mismo código duplicado |
| Recorrer columnas 4 → 0 | LSD: lo menos significativo primero, para que lo más significativo mande al final |

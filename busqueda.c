#include "busqueda.h"

// Búsqueda Secuencial Estándar
int busqueda_secuencial(Deportista *arreglo, int cantidad, int id_buscar) {
    for (int i = 0; i < cantidad; i++) {
        if (arreglo[i].id == id_buscar) {
            return i; // Encontrado
        }
    }
    return -1; // No encontrado
}

// Búsqueda Binaria Iterativa por ID
int busqueda_binaria_iterativa(Deportista *arreglo, int cantidad, int id_buscar) {
    int izquierda = 0;
    int derecha = cantidad - 1;
    while (izquierda <= derecha) {
        int medio = izquierda + (derecha - izquierda) / 2;
        if (arreglo[medio].id == id_buscar) {
            return medio; // Encontrado
        }
        if (arreglo[medio].id < id_buscar) {
            izquierda = medio + 1;
        } else {
            derecha = medio - 1;
        }
    }
    return -1; // No encontrado
}
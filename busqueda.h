#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include "deportista.h" // Necesita la estructura Deportista

// Búsqueda Secuencial
int busqueda_secuencial(Deportista *arreglo, int cantidad, int id_buscar);

// Búsqueda Binaria Iterativa por ID
int busqueda_binaria_iterativa(Deportista *arreglo, int cantidad, int id_buscar);

#endif // BUSQUEDA_H
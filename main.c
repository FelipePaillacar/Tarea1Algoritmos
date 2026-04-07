#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deportista.h"

#define MAX_DEPORTISTAS 10000
#define CANTIDAD_GENERAR 10000// Cantidad de datos a generar para las pruebas
#define ARCHIVO_DATOS "datos.csv"

void mostrar_menu() {
    printf("\n========== MENU PRINCIPAL ==========\n");
    printf("1. Generar, mezclar y guardar datos\n");
    printf("2. Cargar datos desde archivo\n");
    printf("3. Mostrar todos los registros\n");
    printf("4. Ordenar registros (Submenu)\n");
    printf("5. Buscar deportista por ID\n");
    printf("6. Mostrar ranking (Mejores N deportistas)\n");
    printf("7. Salir\n");
    printf("====================================\n");
    printf("Seleccione una opcion: ");
}

void menu_ordenamiento() {
    printf("\n--- ORDENAR POR ---\n");
    printf("1. ID\n");
    printf("2. Nombre\n");
    printf("3. Equipo\n");
    printf("4. Puntaje\n");
    printf("5. Competencias\n");
    printf("Seleccione campo a ordenar: ");
}

// Implementación de Bubble Sort optimizado para generar el ranking (Puntaje Descendente)
void ordenar_ranking_descendente(Deportista *arreglo, int cantidad) {
    for (int i = 0; i < cantidad - 1; i++) {
        int intercambiado = 0; // Optimización para detectar si ya está ordenado
        for (int j = 0; j < cantidad - i - 1; j++) {
            if (arreglo[j].puntaje < arreglo[j + 1].puntaje) {
                Deportista temp = arreglo[j];
                arreglo[j] = arreglo[j + 1];
                arreglo[j + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break; // Si no hubo cambios, terminamos temprano
    }
}

// Función auxiliar para comparar deportistas según el campo seleccionado
// Retorna > 0 si a > b, < 0 si a < b, 0 si son iguales
int comparar_deportistas(Deportista a, Deportista b, int campo) {
    switch(campo) {
        case 1: return a.id - b.id;
        case 2: return strcmp(a.nombre, b.nombre);
        case 3: return strcmp(a.equipo, b.equipo);
        case 4: return (a.puntaje > b.puntaje) - (a.puntaje < b.puntaje); // Evita problemas de truncamiento con floats
        case 5: return a.competencias - b.competencias;
        default: return 0;
    }
}

// 1. Bubble Sort (Multicampo) con optimización
void bubble_sort(Deportista *arreglo, int cantidad, int campo) {
    for (int i = 0; i < cantidad - 1; i++) {
        int intercambiado = 0;
        for (int j = 0; j < cantidad - i - 1; j++) {
            if (comparar_deportistas(arreglo[j], arreglo[j + 1], campo) > 0) {
                Deportista temp = arreglo[j];
                arreglo[j] = arreglo[j + 1];
                arreglo[j + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break; // Optimización solicitada
    }
}

// 2. Insertion Sort (Estándar)
void insertion_sort(Deportista *arreglo, int cantidad, int campo) {
    for (int i = 1; i < cantidad; i++) {
        Deportista clave = arreglo[i];
        int j = i - 1;
        while (j >= 0 && comparar_deportistas(arreglo[j], clave, campo) > 0) {
            arreglo[j + 1] = arreglo[j];
            j = j - 1;
        }
        arreglo[j + 1] = clave;
    }
}

// 3. Selection Sort (Optimizado para evitar intercambios innecesarios)
void selection_sort(Deportista *arreglo, int cantidad, int campo) {
    for (int i = 0; i < cantidad - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < cantidad; j++) {
            if (comparar_deportistas(arreglo[j], arreglo[min_idx], campo) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) { // Optimización solicitada
            Deportista temp = arreglo[i];
            arreglo[i] = arreglo[min_idx];
            arreglo[min_idx] = temp;
        }
    }
}

// 4. Cocktail Shaker Sort (Estándar)
void cocktail_shaker_sort(Deportista *arreglo, int cantidad, int campo) {
    int intercambiado = 1;
    int inicio = 0;
    int fin = cantidad - 1;
    while (intercambiado) {
        intercambiado = 0;
        for (int i = inicio; i < fin; ++i) { // De izquierda a derecha
            if (comparar_deportistas(arreglo[i], arreglo[i + 1], campo) > 0) {
                Deportista temp = arreglo[i];
                arreglo[i] = arreglo[i + 1];
                arreglo[i + 1] = temp;
                intercambiado = 1;
            }
        }
        if (!intercambiado) break;
        intercambiado = 0;
        fin--;
        for (int i = fin - 1; i >= inicio; --i) { // De derecha a izquierda
            if (comparar_deportistas(arreglo[i], arreglo[i + 1], campo) > 0) {
                Deportista temp = arreglo[i];
                arreglo[i] = arreglo[i + 1];
                arreglo[i + 1] = temp;
                intercambiado = 1;
            }
        }
        inicio++;
    }
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

int main() {
    // Inicializar la semilla para los números aleatorios
    srand(time(NULL));

    Deportista arreglo[MAX_DEPORTISTAS];
    int cantidad_actual = 0;
    int opcion;

    // Intentar cargar datos al iniciar el programa
    cantidad_actual = leer_deportistas_csv(arreglo, MAX_DEPORTISTAS, ARCHIVO_DATOS);
    if (cantidad_actual > 0) {
        printf("Se cargaron %d registros desde '%s' automaticamente.\n", cantidad_actual, ARCHIVO_DATOS);
    }

    do {
        mostrar_menu();
        if (scanf("%d", &opcion) != 1) {
            while(getchar() != '\n'); // Limpiar el buffer si el usuario ingresa letras
            continue;
        }

        switch(opcion) {
            case 1: {
                printf("Generando %d deportistas...\n", CANTIDAD_GENERAR);
                reiniciar_generador_id(); 
                for (int i = 0; i < CANTIDAD_GENERAR; i++) {
                    arreglo[i] = generar_deportista();
                }
                cantidad_actual = CANTIDAD_GENERAR;
                mezclar_deportistas(arreglo, cantidad_actual);
                guardar_deportistas_csv(arreglo, cantidad_actual, ARCHIVO_DATOS);
                printf("Datos generados y guardados en '%s'.\n", ARCHIVO_DATOS);
                break;
            }
            case 2: {
                cantidad_actual = leer_deportistas_csv(arreglo, MAX_DEPORTISTAS, ARCHIVO_DATOS);
                if (cantidad_actual > 0) {
                    printf("Se cargaron %d registros exitosamente.\n", cantidad_actual);
                } else {
                    printf("No se pudo cargar o el archivo '%s' esta vacio.\n", ARCHIVO_DATOS);
                }
                break;
            }
            case 3: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados en memoria.\n");
                    break;
                }
                printf("\n--- REGISTROS ACTUALES ---\n");
                for (int i = 0; i < cantidad_actual; i++) {
                    imprimir_deportista(arreglo[i]);
                }
                break;
            }
            case 4: {
                if (cantidad_actual == 0) {
                    printf("No hay datos para ordenar.\n");
                    break;
                }
                int campo;
                menu_ordenamiento();
                scanf("%d", &campo);
                if (campo < 1 || campo > 5) {
                    printf("Campo invalido.\n");
                    break;
                }

                int algoritmo;
                printf("\n--- ALGORITMO ---\n");
                printf("1. Bubble Sort\n");
                printf("2. Insertion Sort\n");
                printf("3. Selection Sort\n");
                printf("4. Cocktail Shaker Sort\n");
                printf("Seleccione algoritmo: ");
                scanf("%d", &algoritmo);

                // Medición de tiempo (Útil para el análisis empírico del PDF)
                clock_t inicio_tiempo = clock();

                switch(algoritmo) {
                    case 1: bubble_sort(arreglo, cantidad_actual, campo); break;
                    case 2: insertion_sort(arreglo, cantidad_actual, campo); break;
                    case 3: selection_sort(arreglo, cantidad_actual, campo); break;
                    case 4: cocktail_shaker_sort(arreglo, cantidad_actual, campo); break;
                    default: printf("Algoritmo invalido.\n"); break;
                }

                if (algoritmo >= 1 && algoritmo <= 4) {
                    clock_t fin_tiempo = clock();
                    double tiempo_gastado = (double)(fin_tiempo - inicio_tiempo) / CLOCKS_PER_SEC;
                    printf("\n[OK] Registros ordenados en %f segundos.\n", tiempo_gastado);
                    printf("Puede usar la opcion 3 del menu para ver los resultados.\n");
                }
                break;
            }
            case 5: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }
                int id_buscar, metodo;
                printf("Ingrese el ID a buscar: ");
                scanf("%d", &id_buscar);
                
                printf("\n--- METODO DE BUSQUEDA ---\n");
                printf("1. Busqueda Secuencial\n");
                printf("2. Busqueda Binaria Iterativa (Requiere ordenar por ID)\n");
                printf("Seleccione metodo: ");
                scanf("%d", &metodo);

                int indice_encontrado = -1;
                clock_t inicio_tiempo = clock();

                if (metodo == 1) {
                    // Búsqueda Secuencial Estándar
                    for (int i = 0; i < cantidad_actual; i++) {
                        if (arreglo[i].id == id_buscar) {
                            indice_encontrado = i;
                            break;
                        }
                    }
                } else if (metodo == 2) {
                    // Búsqueda Binaria requiere arreglo ordenado
                    printf("[!] Ordenando arreglo por ID internamente para aplicar Busqueda Binaria...\n");
                    bubble_sort(arreglo, cantidad_actual, 1);
                    indice_encontrado = busqueda_binaria_iterativa(arreglo, cantidad_actual, id_buscar);
                } else {
                    printf("Metodo invalido.\n");
                    break;
                }

                clock_t fin_tiempo = clock();
                double tiempo_gastado = (double)(fin_tiempo - inicio_tiempo) / CLOCKS_PER_SEC;

                if (indice_encontrado != -1) {
                    printf("\n¡Deportista encontrado en %f segundos!\n", tiempo_gastado);
                    imprimir_deportista(arreglo[indice_encontrado]);
                } else {
                    printf("\nNo se encontro un deportista con el ID %d (Tiempo de busqueda: %f segundos).\n", id_buscar, tiempo_gastado);
                }
                break;
            }
            case 6: {
                if (cantidad_actual == 0) {
                    printf("No hay datos cargados.\n");
                    break;
                }
                int top_n;
                printf("Ingrese la cantidad (N) para el ranking: ");
                scanf("%d", &top_n);
                if (top_n > cantidad_actual) top_n = cantidad_actual;
                if (top_n <= 0) break;

                ordenar_ranking_descendente(arreglo, cantidad_actual);
                
                printf("\n--- RANKING TOP %d ---\n", top_n);
                for (int i = 0; i < top_n; i++) {
                    imprimir_deportista(arreglo[i]);
                }
                break;
            }
            case 7:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Intente nuevamente.\n");
        }
    } while (opcion != 7);

    return 0;
}

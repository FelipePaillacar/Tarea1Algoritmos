#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deportista.h"
#include "menu.h"
#include "ordenamiento.h"
#include "busqueda.h"

#define MAX_DEPORTISTAS 10000 
#define CANTIDAD_GENERAR 10000
#define ARCHIVO_DATOS "datos.csv"

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
                int tipo_generacion;
                menu_generacion_datos(); // Mostrar el nuevo submenú
                if (scanf("%d", &tipo_generacion) != 1) {
                    while(getchar() != '\n');
                    printf("Entrada invalida.\n");
                    break;
                }

                printf("Generando %d deportistas...\n", CANTIDAD_GENERAR);
                
                switch(tipo_generacion) {
                    case 1: // Aleatorios (mezclados)
                        reiniciar_generador_id();
                        for (int i = 0; i < CANTIDAD_GENERAR; i++) {
                            arreglo[i] = generar_deportista();
                        }
                        cantidad_actual = CANTIDAD_GENERAR;
                        mezclar_deportistas(arreglo, cantidad_actual);
                        printf("Datos aleatorios generados y mezclados.\n");
                        break;
                    case 2: // Ordenados por ID (Mejor Caso)
                        generar_datos_ordenados(arreglo, CANTIDAD_GENERAR);
                        cantidad_actual = CANTIDAD_GENERAR;
                        printf("Datos ordenados por ID generados (Mejor Caso).\n");
                        break;
                    case 3: // Inversamente ordenados por ID (Peor Caso)
                        generar_datos_inversos(arreglo, CANTIDAD_GENERAR);
                        cantidad_actual = CANTIDAD_GENERAR;
                        printf("Datos inversamente ordenados por ID generados (Peor Caso).\n");
                        break;
                    default:
                        printf("Opcion de generacion invalida.\n");
                        break;
                }

                if (cantidad_actual > 0) { // Solo guardar si se generaron datos
                    guardar_deportistas_csv(arreglo, cantidad_actual, ARCHIVO_DATOS);
                    printf("Datos guardados en '%s'.\n", ARCHIVO_DATOS);
                }
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
                    indice_encontrado = busqueda_secuencial(arreglo, cantidad_actual, id_buscar);
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

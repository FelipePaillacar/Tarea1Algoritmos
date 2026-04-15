#include "menu.h"
#include <stdio.h>

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

// Nuevo menú para la generación de datos
void menu_generacion_datos() {
    printf("\n--- GENERACION DE DATOS ---\n");
    printf("1. Generar datos aleatorios (mezclados)\n");
    printf("2. Generar datos ordenados por ID (Mejor Caso)\n");
    printf("3. Generar datos inversamente ordenados por ID (Peor Caso)\n");
    printf("Seleccione tipo de generacion: ");
}
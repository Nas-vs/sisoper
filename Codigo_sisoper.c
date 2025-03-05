//Integrantes:
//Sebastian Mejia Arana
//Juan David Rivera Barona
//Nasly Vanesa Valencia Salamanca


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Variables globales (Segmento de Datos)
int global_var = 42;
static int static_global_var = 100;

// Función para mostrar direcciones de memoria de distintos segmentos
void mostrar_segmentos() {
    int local_var = 10; // Variable local (Pila)
    static int static_local_var = 20; // Variable estática local (Datos)
    int *heap_var = (int*) malloc(sizeof(int)); // Variable en Heap

    if (!heap_var) {
        printf("Error al asignar memoria en Heap.\n");
        return;
    }

    printf("\n--- Direcciones de memoria ---\n");
    printf("Variable global: %p\n", (void*)&global_var);
    printf("Variable estática global: %p\n", (void*)&static_global_var);
    printf("Variable local: %p\n", (void*)&local_var);
    printf("Variable estática local: %p\n", (void*)&static_local_var);
    printf("Variable en Heap: %p\n", (void*)heap_var);

    free(heap_var); // Liberar memoria asignada en Heap
}

// Función para mostrar el consumo de memoria del proceso usando /proc/self/status
void mostrar_consumo_memoria() {
    printf("\n--- Consumo de Memoria ---\n");
    FILE *fp = fopen("/proc/self/status", "r");
    if (!fp) {
        printf("Error al leer /proc/self/status\n");
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "VmSize") || strstr(line, "VmRSS")) {
            printf("%s", line);
        }
    }
    fclose(fp);
}

// Función para asignar memoria dinámica
typedef struct {
    int *arr;
    size_t size;
} MemoriaDinamica;

void asignar_memoria_dinamica(MemoriaDinamica *mem, size_t size) {
    if (mem->arr) {
        printf("Ya hay memoria asignada. Libérala primero.\n");
        return;
    }

    printf("Consumo de memoria antes de la asignación:\n");
    mostrar_consumo_memoria();

    mem->arr = (int*) malloc(size * sizeof(int));
    if (!mem->arr) {
        printf("Error al asignar memoria.\n");
        return;
    }

    // Llenar el arreglo con datos
    for (size_t i = 0; i < size; i++) {
        mem->arr[i] = i;
    }

    mem->size = size;
    printf("Se asignaron %zu enteros en memoria dinámica.\n", size);
    printf("Consumo de memoria después de la asignación:\n");
    mostrar_consumo_memoria();
}

void liberar_memoria_dinamica(MemoriaDinamica *mem) {
    if (!mem->arr) {
        printf("No hay memoria asignada para liberar.\n");
        return;
    }

    free(mem->arr);
    mem->arr = NULL;
    mem->size = 0;
    printf("Memoria liberada.\n");
    printf("Consumo de memoria después de liberar:\n");
    mostrar_consumo_memoria();
}

// Menú interactivo
void menu() {
    int opcion;
    size_t size;
    MemoriaDinamica mem = {NULL, 0};

    do {
        printf("\n--- Menú de Gestión de Memoria ---\n");
        printf("1. Mostrar direcciones de memoria\n");
        printf("2. Mostrar consumo de memoria\n");
        printf("3. Asignar memoria dinámica\n");
        printf("4. Liberar memoria dinámica\n");
        printf("5. Salir\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); // Limpiar buffer de entrada

        switch (opcion) {
            case 1:
                mostrar_segmentos();
                break;
            case 2:
                mostrar_consumo_memoria();
                break;
            case 3:
                if (!mem.arr) {
                    printf("Ingrese la cantidad de enteros a asignar en memoria: ");
                    scanf("%zu", &size);
                    getchar(); // Limpiar buffer de entrada
                    asignar_memoria_dinamica(&mem, size);
                } else {
                    printf("Ya hay memoria asignada. Libérala antes de asignar otra.\n");
                }
                break;
            case 4:
                liberar_memoria_dinamica(&mem);
                break;
            case 5:
                liberar_memoria_dinamica(&mem);
                printf("Saliendo...\n");
                break;
            default:
                printf("Opción no válida.\n");
        }
    } while (opcion != 5);
}

int main() {
    menu();
    return 0;
}

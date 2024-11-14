#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

#define SIZE 100000
#define RANGE 10000

// Función que intercambia los valores de dos parámetros con pasaje por referencia
void swap(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

// Función para determinar números primos
bool isPrime(int n) {
    int i;

    if (n < 2) {
        return false;
    }

    for (i = 2; i * i <= n; i++) {
        if (n % i == 0) {
            return false;
        }
    }

    return true;
}

// Función para fusionar las secciones
void mergeSections(int *array, int *displacement, int *segmentSize, int processNum) {
    int totalSize = 0;
    int i;

    for (i = 0; i < processNum; i++) {
        totalSize += segmentSize[i];
    }
    
    int *tempArray = (int *)malloc(totalSize * sizeof(int));
    int *index = (int *)malloc(processNum * sizeof(int));
    for (i = 0; i < processNum; i++) {
        index[i] = displacement[i];
    }
    
    int count = 0;
    while(count < totalSize) {
        int minIndex = -1;
        for (i = 0; i < processNum; i++) {
            if (index[i] < displacement[i] + segmentSize[i]) {
                if (minIndex == -1 || array[index[i]] < array[index[minIndex]]) {
                    minIndex = i;
                }
            }
        }
        tempArray[count++] = array[index[minIndex]];
        index[minIndex]++;
    }

    for (i = 0; i < totalSize; i++) {
        array[i] = tempArray[i];
    }

    free(tempArray);
    free(index);
}

int partition(int array[], int low, int high) {
    int pivot = array[high];    // Pivote ubicado en el mayor elemento
    int i = (low - 1);  // (-1)
    int j;

    // Recorre el arreglo buscando ubicar los elementos mayores y menores al pivote
    for(j = low; j < high; j++) {
        if(array[j] <= pivot) {
            i++;
            swap(&array[i], &array[j]);
        }
    }

    // Termina de acomodar el pivote
    swap(&array[i + 1], &array[high]);
    return (i + 1); // Devuelve el índice del pivote
}

void quickSort(int array[], int low, int high) {
    if (low < high) {
        // Devuelve el index del pivote
        int pivotIndex = partition(array, low, high);

        // Ordena los elementos a la izquierda del pivote
        quickSort(array, low, pivotIndex - 1);

        // Ordena los elementos a la derecha del pivote
        quickSort(array, pivotIndex + 1, high);
    }
}

// Imprime el array
void printArray(int array[], int size) {
    int i;
    for (i = 0; i < size; ++i) {
        printf("%d  ", array[i]);
    }
    printf("\n");
}

int main(int argc, char **argv) {
    int processRank, processNum;
    int *localArray = NULL; // Arreglo que cada proceso va a utilizar
    int *array = NULL;  // Arreglo general
    int i;  // Este será un index genérico que usaremos para recorrer todos los bucles
    double startTime, endTime, totalTime;
    int primeNumbers = 0;

    MPI_Init(&argc, &argv); // Inicia MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &processRank);    // Obtiene el rango del proceso
    MPI_Comm_size(MPI_COMM_WORLD, &processNum); // Obtiene el tamaño del proceso
    
    printf("Ejecución del proceso %d iniciada...\n", processRank);

    int localSize = SIZE / processNum;   // Tamaño que tendrá cada arreglo
    int remainder = SIZE % processNum;  // Resto de elementos no asignados

    int *segmentSize = (int*)malloc(processNum * sizeof(int));   // Guarda el tamaño de cada segmento
    int *displacement = (int*)malloc(processNum * sizeof(int));   // Almacena el desplazamiento de cada segmento en el arreglo completo

    for (i = 0; i < processNum; i++) {
        segmentSize[i] = (i < remainder) ? localSize + 1 : localSize;
        displacement[i] = (i == 0) ? 0 : displacement[i - 1] + segmentSize[i - 1];
    }

    // Si este proceso es el proceso 0 (proceso raíz)
    // Se encarga de la parte secuencial: asignar valores a los elementos del array
    if(processRank == 0) {
        // Temporizador
        startTime = MPI_Wtime();
        
        // Reserva memoria para el arreglo completo según SIZE
        array = (int*)malloc(SIZE * sizeof(int));

        // Imprimimos el arreglo desordenado
        for(i = 0; i < SIZE; i++) {
            array[i] = rand() % RANGE;
            if(isPrime(array[i])){
                primeNumbers++;
            }
        }

        printf("Cantidad de números primos: %d\n", primeNumbers);
    }

    #pragma region 
    // Asigna la memoria determinada para el arreglo local
    localArray = (int*)malloc(localSize * sizeof(int));

    // Distribuir los datos entre los procesos
    MPI_Scatterv(array, segmentSize, displacement, MPI_INT, localArray, segmentSize[processRank], MPI_INT, 0, MPI_COMM_WORLD);

    // Cada proceso ordena su parte del array
    quickSort(localArray, 0, segmentSize[processRank] - 1);

    // Recolectar los datos ordenados en el proceso root
    MPI_Gatherv(localArray, segmentSize[processRank], MPI_INT, array, segmentSize, displacement, MPI_INT, 0, MPI_COMM_WORLD);
    #pragma endregion

    if(processRank == 0) {
        mergeSections(array, displacement, segmentSize, processNum);    // Fusiona todos los arreglos

        endTime = MPI_Wtime();

        /* printf("Array ordenado: ");
        for (i = 0; i < SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n"); */
        
        totalTime = endTime - startTime;

        printf("Tiempo de ejecución: %lf segundos.\n", totalTime);

        // Liberamos la memoria del proceso root
        free(array);
    }

    // Liberamos la memoria del proceso actual
    free(localArray);

    printf("Ejecución del proceso %d finalizada.\n", processRank);

    MPI_Finalize();
    return 0;
}


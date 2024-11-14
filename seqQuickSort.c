#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <mpi.h>

#define SIZE 10000000
#define RANGE 10000

void swap(int *a, int *b)
{
  int t = *a;
  *a = *b;
  *b = t;
}

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

int partition(int array[], int low, int high)
{
  int pivot = array[high];
  int i = (low - 1);
  int j;

  for (j = low; j < high; j++)
  {
    if (array[j] <= pivot)
    {
      i++;
      swap(&array[i], &array[j]);
    }
  }

  swap(&array[i + 1], &array[high]);

  return (i + 1);
}

void quickSort(int array[], int low, int high)
{
  if (low < high)
  {
    int pi = partition(array, low, high);

    quickSort(array, low, pi - 1);

    quickSort(array, pi + 1, high);
  }
}

void printArray(int array[], int size)
{
  int i;
  for (i = 0; i < size; ++i)
  {
    printf("%d  ", array[i]);
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  int processRank, processNum;
  int i;
  int primeNumbers = 0;
  int *array = NULL;
  double startTime, endTime, totalTime;

  MPI_Init(&argc, &argv); // Inicia MPI
  MPI_Comm_rank(MPI_COMM_WORLD, &processRank);    // Obtiene el rango del proceso
  MPI_Comm_size(MPI_COMM_WORLD, &processNum); // Obtiene el tamaño del proceso

  array = (int*)malloc(SIZE * sizeof(int));

  if(processRank == 0){
    startTime = MPI_Wtime();

    // Imprimimos el arreglo desordenado
    for(i = 0; i < SIZE; i++) {
      array[i] = rand() % RANGE;
      if(isPrime(array[i])) {
        primeNumbers++;
      }
    }

    printf("Cantidad de numeros primos: %d\n", primeNumbers);

    quickSort(array, 0, SIZE - 1);

    endTime = MPI_Wtime();
    totalTime = endTime - startTime;
    printf("Tiempo de ejecución: %lf segundos.\n", totalTime);
  }
  

  MPI_Finalize();
  return 0;
}
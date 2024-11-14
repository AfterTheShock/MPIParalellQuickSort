# Algoritmo QuickSort en Paralelo con C y MPI
## Introducción Proyecto de Computación Paralela
Este proyecto fue desarrollado para la materia Computación Paralela, de la UTN FRM. Se implementa el algoritmo de ordenamiento QuickSort en paralelo con la librería MPI y el lenguaje de programación C.
## Implementación del Algoritmo QuickSort en Paralelo
1. El conjunto de elementos a ordenar se divide y asigna entre los procesos de forma equitativa.
2. Cada proceso ejecuta el algoritmo QuickSort de forma secuencial, ordenando su parte del arreglo.
3. El proceso raíz fusiona y ordena los distintos fragmentos.
## Compilación y Ejecución
### Compilación
Para compilar el programa con el compilador de MPI:
```
mpicc parallelQuickSort.c -o paralellQuickSort
```
### Ejecución
Para ejecutar el programa con una cantidad de nodos determinada:
```
mpiexec -n [cantidad de nodos] ./paralellQuickSort
```
## Tecnologías
[![C](https://skillicons.dev/icons?i=c)](https://skillicons.dev)

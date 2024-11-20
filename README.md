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
## Análisis de Resultados
Análisis de los resultados con diferentes cantidades de nodos y 50 millones de datos. Todas estas pruebas fueron realizadas en el cluster de la UTN FRM.

<div align="center">
<img src="https://res.cloudinary.com/dkxjaqheh/image/upload/v1732086668/phumkv3rbuwfnej3ujem.jpg" width="500" height="325">


|Nodos           |Tiempo (seg)|Eficiencia(%)   |
|:--------------:|:----------:|:--------------:|
|1 (Secuencial)  |2514,045683 |-               |
|2               |1112,670131 |112,97%         |
|5               |230,797155  |217,86%         |
|8               |197,513482  |159,11%         |
|10              |109,732774  |299,29%         |

</div>
<hr></hr>

Queda demostrado el uso del procesamiento paralelo para la optimización de algoritmos de ordenamiento.

## Tecnologías
<div align="center">
<img src="https://skillicons.dev/icons?i=c">
</div>

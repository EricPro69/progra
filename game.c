#include <stdlib.h>
#include "structures.h"
#include "map.h"

// Funcion de prueba del juego - crea e imprime una matriz 5x5
int game() {
    int matriz[5][5];  // Matriz de 5x5
    int contador = 1;  // Contador para llenar la matriz

    // Llenar la matriz con numeros consecutivos
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            matriz[i][j] = contador++;
        }
    }

    // Imprimir la matriz en la consola
    printf("Matriz 5x5:\n");
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            printf("%d\t", matriz[i][j]);
        }
        printf("\n");
    }

    return 0;
}


//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon
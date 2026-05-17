// RPG de un perro que pelea contra salchichas
// Proyecto final de programacion
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "structures.h"
#include "map.h"

void mostrarMapa(Mapa *mapa); // Declaración de función para mostrar el mapa

static void manejarCtrlC(int signo) {
    (void)signo;
    printf("\nCtrl+C recibido. Saliendo...\n");
    exit(0);
}

// Función principal - punto de entrada del programa
int main(void){
    signal(SIGINT, manejarCtrlC);
    return level(); // Inicia el primer nivel del juego
}








//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon
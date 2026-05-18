#ifndef MAP_H
#define MAP_H
#include <stdio.h>
#include "structures.h"
#include "combat.h"

#define MAP_ROWS 8
#define MAP_COLS 19

// Prototipos de funciones relacionadas con mapas y niveles
int level(void);  // Funcion principal del nivel
int cargarMapa(int x, int y, Mapa *mapa);  // Cargar mapa desde archivo
int moverJugador(int *playerX, int *playerY, Mapa *mapa);  // Procesar movimiento del jugador
void mostrarMapa(Mapa *mapa);  // Mostrar mapa sin jugador
void mostrarMapaConJugador(Mapa *mapa, int playerX, int playerY);  // Mostrar mapa con jugador

#endif

//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon
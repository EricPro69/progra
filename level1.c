#include <stdio.h>
#include <stdlib.h>
#include <conio.h>  // Para funciones de entrada de teclado
#include "structures.h"
#include "map.h"

#define MAP_SIZE 7           // Tamaño del mapa
#define PLAYER_SYMBOL 'P'    // Símbolo para representar al jugador

// Función principal del primer nivel
// Presenta la historia y espera entradas del jugador
int level(void){
    // Mostrar la trama del juego
    printf("Bienvenido a el regreso a casa de salchichin\n");
    printf("Salchichin y su familia se mudaron a una nueva casa, sin ver que en el refri dejaron algo.\n");
    printf("Un paquete de salchichas!\n");
    printf("Pero el paquete de salchichas estaba caducado y estas mutaron\n");
    printf("Ahora salchichin debe enfrentarse a sus enemigos para poder cuidar a su familia\n");
    _getch(); // Pausa antes de iniciar el nivel

    Mapa mapaActual;  // Estructura para almacenar el mapa actual
    int playerX = 1;
    int playerY = 1;
    int mapX = 0;
    int mapY = 0;
    int continuar = 1;

    cargarMapa(mapX, mapY, &mapaActual);

    while (continuar) {
        system("cls");
        mostrarMapaConJugador(&mapaActual, playerX, playerY);
        continuar = moverJugador(&playerX, &playerY, &mapaActual);

        if (continuar == 2) {
            int nextMapX = mapX;
            int nextMapY = mapY;

            // Determinar la dirección del cambio de mapa basado en la posición de la puerta
            if (playerX == 0) {
                nextMapX = mapX - 1;
                playerX = 18;  // Aparecer en el borde derecho del nuevo mapa
            } else if (playerX == 18) {
                nextMapX = mapX + 1;
                playerX = 0;   // Aparecer en el borde izquierdo del nuevo mapa
            } else if (playerY == 0) {
                nextMapY = mapY - 1;
                playerY = 7;   // Aparecer en el borde inferior del nuevo mapa
            } else if (playerY == 7) {
                nextMapY = mapY + 1;
                playerY = 0;   // Aparecer en el borde superior del nuevo mapa
            }

            if (cargarMapa(nextMapX, nextMapY, &mapaActual)) {
                mapX = nextMapX;
                mapY = nextMapY;
            } else {
                // Si no existe el nuevo mapa, mantener al jugador en el mapa actual
                // Resetear posición si es necesario, pero como está en puerta, quizás no
                printf("No hay mapa en esa dirección.\n");
                _getch();  // Pausa para mostrar el mensaje
            }

            continuar = 1;
        }
    }

    return 0;
}

// Carga un mapa desde un archivo de texto
// Parámetros: x, y (coordenadas del mapa), *mapa (puntero donde guardar datos)
// Retorna: 1 si el mapa se cargó correctamente, 0 si no existe
int cargarMapa(int x, int y, Mapa *mapa) {
    char nombreArchivo[20];  // Nombre del archivo del mapa
    
    // Construir el nombre del archivo basado en las coordenadas
    sprintf(nombreArchivo, "mapas/mapa%d_%d.txt", x, y);

    // Intentar abrir el archivo
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo cargar el mapa: %s\n", nombreArchivo);
        return 0;
    }
    
    printf("Cargando mapa: %s\n", nombreArchivo);
    
    // Leer 8 líneas del archivo (dimensión del mapa)
    for (int i = 0; i < 8; i++) {
        if (fgets(mapa->celdas[i], sizeof(mapa->celdas[i]), archivo) == NULL) break;
    }
    
    fclose(archivo);  // Cerrar el archivo
    return 1;
}

// Muestra el mapa en la consola con la posición del jugador
// Parámetros: *mapa (puntero al mapa), playerX, playerY (posición del jugador)
void mostrarMapaConJugador(Mapa *mapa, int playerX, int playerY) {
    // Recorrer cada celda del mapa (8 filas x 19 columnas)
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 19; j++) {
            // Si estamos en la posición del jugador, mostrar 'P'
            if (i == playerY && j == playerX) {
                printf("P");
            } else {
                printf("%c", mapa->celdas[i][j]);
            }
        }
        printf("\n");
    }
}

// Procesa el movimiento del jugador basado en la entrada del teclado
// Parámetros: *playerX, *playerY (posición actual), *mapa (mapa actual)
// Retorna: 0 si el jugador quiere salir, 1 si continúa en el mismo mapa,
// 2 si el jugador llega a una puerta y debe cargar el siguiente mapa
int moverJugador(int *playerX, int *playerY, Mapa *mapa){
    int key = getch();  // Obtener tecla presionada

    int newX = *playerX;  // Nueva posición X
    int newY = *playerY;  // Nueva posición Y
    int moveDir = 0; // 1=arriba,2=abajo,3=izq,4=der

    // Si es una tecla especial (flecha)
    if (key == 0 || key == 224) {
        key = getch();  // Obtener el código de la flecha
        switch (key) {
            case 72:  // Flecha arriba
                newY--;
                moveDir = 1;
                break;
            case 80:  // Flecha abajo
                newY++;
                moveDir = 2;
                break;
            case 75:  // Flecha izquierda
                newX--;
                moveDir = 3;
                break;
            case 77:  // Flecha derecha
                newX++;
                moveDir = 4;
                break;
            default:
                break;
        }
    } 
    // Si el usuario presiona 'Q', salir
    else {
        if (key == 'q' || key == 'Q') {
            return 0;  // Salir del juego
        }
    }

    // Verificar si el movimiento es válido (dentro de límites y no hay obstáculo)
    if (newX >= 0 && newX < 19 && newY >= 0 && newY < 8 && mapa->celdas[newY][newX] != '#') {
        *playerX = newX;
        *playerY = newY;
    }

    // Si el jugador alcanza una puerta: '|' (cualquiera) o '-' (solo vertical)
    if (newX >= 0 && newX < 19 && newY >= 0 && newY < 8) {
        if (mapa->celdas[newY][newX] == '|') {
            return 2;  // Cambiar de mapa (entrada/salida horizontal)
        }
        if (mapa->celdas[newY][newX] == '-' && (moveDir == 1 || moveDir == 2)) {
            return 2;  // Cambiar de mapa (entrada/salida vertical)
        }
    }

    return 1;  // Continuar en el mismo mapa
}
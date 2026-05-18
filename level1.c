#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>  // Para funciones de entrada de teclado
#include "structures.h"
#include "map.h"
#include "combat.h"   // NUEVO: sistema de combate

static int esCeldaValidaParaEnemigo(Mapa *mapa, int x, int y, int playerX, int playerY);
static void colocarEnemigosAleatorios(Mapa *mapa, int playerX, int playerY);
static int checarKonami(int tecla);
static int mostrarMapaEspecial(const char *ruta);

// Funcion principal del primer nivel
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

    // NUEVO: Inicializar jugador con stats base
    Jugador jugador;
    inicializarJugador(&jugador);

    cargarMapa(mapX, mapY, &mapaActual);
    colocarEnemigosAleatorios(&mapaActual, playerX, playerY);

    while (continuar) {
        system("cls");

        // NUEVO: Mostrar stats del jugador arriba del mapa
        printf("HP: %d/%d  |  XP: %.0f  |  Nivel: %d  |  Dano: %d\n",
               jugador.hp, jugador.hpMax, jugador.xp, jugador.nivel, jugador.dmg);

        mostrarMapaConJugador(&mapaActual, playerX, playerY);
        continuar = moverJugador(&playerX, &playerY, &mapaActual);

        if (continuar == 3) {
            if (cargarMapa(-1, 1, &mapaActual)) {
                system("cls");
                mostrarMapaEspecial("mapas/mapa-1_1.txt");
                printf("\n¡¡Codigo Konami activado!!\n");
                printf("Has entrado al mapa secreto -1_1.\n");
                printf("Felicitaciones, Salchichin ha despertado de su sueño.\n");
                printf("Presiona cualquier tecla para salir...\n");
                _getch();
                return 0;
            } else {
                printf("\nMapa secreto no encontrado.\n");
                _getch();
                continuar = 1;
            }
        }

        // NUEVO: Verificar si el jugador piso un jefe 'J' o un enemigo normal 'E'
        if (mapaActual.celdas[playerY][playerX] == 'J' || mapaActual.celdas[playerY][playerX] == 'E') {
            int tipoEnemigo = (mapaActual.celdas[playerY][playerX] == 'J') ? 2 : 0;
            Enemigo enemigo = crearEnemigo(tipoEnemigo);
            int resultado = iniciarCombate(&jugador, &enemigo);

            if (resultado == 0) {
                printf("\nGame Over. Salchichin no pudo proteger a su familia...\n");
                _getch();
                return 0;
            }

            if (resultado == 1) {
                if (tipoEnemigo == 2) {
                    printf("\nHas derrotado al Jefe! Preparando el mapa de la victoria...\n");
                    _getch();
                    if (mostrarMapaEspecial("mapas/mapaVictoria.txt")) {
                        printf("\n¡Felicidades! Has llegado al mapa de la victoria.\n");
                        printf("Presiona cualquier tecla para cerrar el juego...\n");
                        _getch();
                        return 0;
                    }
                }
                mapaActual.celdas[playerY][playerX] = '-';
            }
        }

        // NUEVO: Verificar si el jugador piso una 'C' (cofre)
        if (mapaActual.celdas[playerY][playerX] == 'C') {
            int sorpresa = rand() % 100;
            if (sorpresa < 35) {
                int curacion = 20 + rand() % 21; // 20 a 40
                printf("\n¡Encontraste un cofre con un vendaje! Cura %d HP.\n", curacion);
                jugador.hp += curacion;
                if (jugador.hp > jugador.hpMax) jugador.hp = jugador.hpMax;
                printf("HP: %d/%d\n", jugador.hp, jugador.hpMax);
            } else if (sorpresa < 60) {
                Item itemCofre;
                strcpy(itemCofre.nombre, "Diente afilado");
                itemCofre.tipo  = 1;   // arma
                itemCofre.valor = 3 + rand() % 3;  // +3 a +5 dmg
                printf("\n¡Cofre encontrado! Equipo %s (+%d de daño).\n", itemCofre.nombre, itemCofre.valor);
                agregarItem(&jugador, itemCofre);
            } else if (sorpresa < 85) {
                Item itemCofre;
                strcpy(itemCofre.nombre, "Chaleco de Hueso");
                itemCofre.tipo  = 2;   // armadura
                itemCofre.valor = 5;
                printf("\n¡Cofre encontrado! Equipo %s (+5%% esquive).\n", itemCofre.nombre);
                agregarItem(&jugador, itemCofre);
            } else {
                int xpBonus = 20 + rand() % 31; // 20 a 50
                printf("\n¡Cofre misterioso! Ganaste %d XP instantanea.\n", xpBonus);
                ganarXP(&jugador, xpBonus);
            }
            mapaActual.celdas[playerY][playerX] = '-';  // Cofre ya abierto
            _getch();
        }

        if (continuar == 2) {
            int nextMapX = mapX;
            int nextMapY = mapY;

            // Determinar la direccion del cambio de mapa basado en la posicion de la puerta
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
                colocarEnemigosAleatorios(&mapaActual, playerX, playerY);
                mapX = nextMapX;
                mapY = nextMapY;
            } else {
                // Si no existe el nuevo mapa, mantener al jugador en el mapa actual
                // Resetear posicion si es necesario, pero como esta en puerta, quizas no
                printf("No hay mapa en esa direccion.\n");
                _getch();  // Pausa para mostrar el mensaje
            }

            continuar = 1;
        }
    }

    return 0;
}

// Carga un mapa desde un archivo de texto
// Parametros: x, y (coordenadas del mapa), *mapa (puntero donde guardar datos)
// Retorna: 1 si el mapa se cargo correctamente, 0 si no existe
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
    
    // Leer MAP_ROWS lineas del archivo (dimension del mapa)
    for (int i = 0; i < MAP_ROWS; i++) {
        if (fgets(mapa->celdas[i], sizeof(mapa->celdas[i]), archivo) == NULL) break;
    }

    // Limpiar posibles enemigos codificados en el mapa original
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            if (mapa->celdas[i][j] == 'E') {
                mapa->celdas[i][j] = '-';
            }
        }
    }
    
    fclose(archivo);  // Cerrar el archivo
    return 1;
}

static int mostrarMapaEspecial(const char *ruta) {
    FILE *archivo = fopen(ruta, "r");
    if (archivo == NULL) {
        printf("No se pudo cargar el mapa especial: %s\n", ruta);
        return 0;
    }

    char linea[64];
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        fputs(linea, stdout);
    }

    fclose(archivo);
    return 1;
}

// Muestra el mapa en la consola con la posicion del jugador
// Parametros: *mapa (puntero al mapa), playerX, playerY (posicion del jugador)
void mostrarMapaConJugador(Mapa *mapa, int playerX, int playerY) {
    // Recorrer cada celda del mapa (MAP_ROWS x MAP_COLS)
    for (int i = 0; i < MAP_ROWS; i++) {
        for (int j = 0; j < MAP_COLS; j++) {
            // Si estamos en la posicion del jugador, mostrar 'P'
            if (i == playerY && j == playerX) {
                printf("P");
            } else {
                printf("%c", mapa->celdas[i][j]);
            }
        }
        printf("\n");
    }
}

static int esCeldaValidaParaEnemigo(Mapa *mapa, int x, int y, int playerX, int playerY) {
    if (x == playerX && y == playerY) {
        return 0;
    }

    if (x < 0 || x >= MAP_COLS || y < 0 || y >= MAP_ROWS) {
        return 0;
    }

    return mapa->celdas[y][x] == '-';
}

static void colocarEnemigosAleatorios(Mapa *mapa, int playerX, int playerY) {
    int colocados = 0;
    int intentos = 0;
    int cantidad = rand() % 4 + 2; // 2 a 5 enemigos normales

    while (colocados < cantidad && intentos < 200) {
        int x = rand() % MAP_COLS;
        int y = rand() % MAP_ROWS;

        if (esCeldaValidaParaEnemigo(mapa, x, y, playerX, playerY)) {
            mapa->celdas[y][x] = 'E';
            colocados++;
        }
        intentos++;
    }
}

static int checarKonami(int tecla) {
    static const int secuenciaKonami[10] = {72, 72, 80, 80, 75, 77, 75, 77, 'b', 'a'};
    static int progreso = 0;

    if (tecla == secuenciaKonami[progreso] ||
        (progreso == 8 && (tecla == 'b' || tecla == 'B')) ||
        (progreso == 9 && (tecla == 'a' || tecla == 'A'))) {
        progreso++;
        if (progreso >= 10) {
            progreso = 0;
            return 1;
        }
        return 0;
    }

    progreso = (tecla == secuenciaKonami[0]) ? 1 : 0;
    return 0;
}

// Procesa el movimiento del jugador basado en la entrada del teclado
// Parametros: *playerX, *playerY (posicion actual), *mapa (mapa actual)
// Retorna: 0 si el jugador quiere salir, 1 si continua en el mismo mapa,
// 2 si el jugador llega a una puerta y debe cargar el siguiente mapa
int moverJugador(int *playerX, int *playerY, Mapa *mapa){
    int key = getch();  // Obtener tecla presionada

    int newX = *playerX;  // Nueva posicion X
    int newY = *playerY;  // Nueva posicion Y
    int moveDir = 0; // 1=arriba,2=abajo,3=izq,4=der

    // Si es una tecla especial (flecha)
    if (key == 0 || key == 224) {
        key = getch();  // Obtener el codigo de la flecha
        if (checarKonami(key)) {
            return 3; // Código Konami completado
        }
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
    // Si el usuario presiona 'Q', salir o si es parte del Konami
    else {
        if (checarKonami(key)) {
            return 3; // Código Konami completado
        }
        if (key == 'q' || key == 'Q') {
            return 0;  // Salir del juego
        }
    }

    // Verificar si el movimiento es valido (dentro de limites y no hay obstaculo)
    if (newX >= 0 && newX < MAP_COLS && newY >= 0 && newY < MAP_ROWS && mapa->celdas[newY][newX] != '#') {
        *playerX = newX;
        *playerY = newY;
    }

    // Si el jugador alcanza una puerta: '|' (cualquiera) o '-' (solo vertical)
    if (newX >= 0 && newX < MAP_COLS && newY >= 0 && newY < MAP_ROWS) {
        if (mapa->celdas[newY][newX] == '|') {
            return 2;  // Cambiar de mapa (entrada/salida horizontal)
        }
        if (mapa->celdas[newY][newX] == '-' && (moveDir == 1 || moveDir == 2)) {
            return 2;  // Cambiar de mapa (entrada/salida vertical)
        }
    }

    return 1;  // Continuar en el mismo mapa
}
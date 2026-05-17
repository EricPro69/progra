#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "structures.h"
#include "combat.h"

// XP necesaria para cada nivel (índice = nivel actual)
static float xpParaNivel[] = {0, 100, 250, 450, 700, 1000};
#define NIVEL_MAX 5

// ─────────────────────────────────────────────
// INICIALIZACIÓN
// ─────────────────────────────────────────────

// Inicializa al jugador con stats base y skills vacías
void inicializarJugador(Jugador *j) {
    j->hp      = 100;
    j->hpMax   = 100;
    j->xp      = 0;
    j->nivel   = 1;
    j->dmg     = 15;
    j->dodge   = 0.10f;  // 10% de esquive base
    j->inv.cantidad = 0;
    j->numSkills    = 0;

    // Skills predefinidas que se van desbloqueando por nivel
    // Se definen todas aquí; desbloquearSkills() las activa según el nivel
    Skill todasLasSkills[] = {
        {"Mordida Fuerte",  "Ataque fuerte, +50% dmg",          2,  8,  0},
        {"Rodar",           "Esquiva el turno, sin daño",        2,  0,  0},
        {"Ladrido Feroz",   "Asusta al enemigo, -1 turno suyo",  3,  0,  0},
        {"Colmillo Rabioso","Ataque brutal, +100% dmg, -5 HP",   4, 15,  5},
        {"Frenesí Final",   "Ataque x2, cuesta 10 HP propios",   5, 20, 10},
    };

    // Guardar las skills en el jugador (bloqueadas hasta que suba de nivel)
    for (int i = 0; i < 5; i++) {
        j->skills[i] = todasLasSkills[i];
    }

    srand((unsigned int)time(NULL));  // Semilla para aleatoriedad
}

// Crea un enemigo según su tipo
// tipo 0 = Salchicha Normal, 1 = Salchicha Podrida, 2 = Salchicha Mutante Jefe
Enemigo crearEnemigo(int tipo) {
    Enemigo e;
    e.hp = e.hpMax = 0;

    if (tipo == 0) {
        strcpy(e.nombre, "Salchicha Normal");
        e.hp = e.hpMax  = 40;
        e.xpReward      = 50.0f;
        e.dmg           = 8;
        e.dodge         = 0.05f;
        e.chanceLoot    = 40;  // 40% de soltar item
        strcpy(e.lootPosible.nombre, "Trozo de Salchicha");
        e.lootPosible.tipo  = 0;  // consumible
        e.lootPosible.valor = 15; // cura 15 HP
    } else if (tipo == 1) {
        strcpy(e.nombre, "Salchicha Podrida");
        e.hp = e.hpMax  = 70;
        e.xpReward      = 100.0f;
        e.dmg           = 14;
        e.dodge         = 0.10f;
        e.chanceLoot    = 55;
        strcpy(e.lootPosible.nombre, "Hueso Resistente");
        e.lootPosible.tipo  = 2;  // armadura
        e.lootPosible.valor = 5;  // reduce 5 de daño recibido (aplicar al usar)
    } else if (tipo == 2) {
        strcpy(e.nombre, "Salchicha Mutante Jefe");
        e.hp = e.hpMax  = 130;
        e.xpReward      = 250.0f;
        e.dmg           = 22;
        e.dodge         = 0.15f;
        e.chanceLoot    = 100; // siempre suelta loot
        strcpy(e.lootPosible.nombre, "Collar de Campeon");
        e.lootPosible.tipo  = 1;  // arma
        e.lootPosible.valor = 10; // +10 dmg permanente al equipar
    }

    return e;
}

// ─────────────────────────────────────────────
// COMBATE
// ─────────────────────────────────────────────

// Muestra HP de jugador y enemigo
void mostrarEstadoCombate(Jugador *j, Enemigo *e) {
    printf("\n=============================\n");
    printf("  Salchichin  HP: %d/%d\n", j->hp, j->hpMax);
    printf("  %s  HP: %d/%d\n", e->nombre, e->hp, e->hpMax);
    printf("=============================\n");
}

// Calcula daño con +-20% de variación aleatoria
int calcularDanio(int dmgBase) {
    int variacion = dmgBase / 5;  // 20% del daño base
    return dmgBase - variacion + (rand() % (variacion * 2 + 1));
}

// Retorna 1 si el personaje esquiva según su probabilidad
int intentarEsquivar(float dodge) {
    float roll = (float)(rand() % 100) / 100.0f;
    return roll < dodge;
}

// Turno del jugador: menú de opciones
void turnoJugador(Jugador *j, Enemigo *e) {
    int opcion = -1;
    printf("\n¿Qué hace Salchichín?\n");
    printf("  1. Atacar\n");
    printf("  2. Usar Skill\n");
    printf("  3. Usar Item\n");
    printf("  4. Huir\n");
    printf("Opción: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        // Ataque básico
        if (intentarEsquivar(e->dodge)) {
            printf("\n%s esquivó el ataque!\n", e->nombre);
        } else {
            int danio = calcularDanio(j->dmg);
            e->hp -= danio;
            if (e->hp < 0) e->hp = 0;
            printf("\nSalchichín atacó por %d de daño!\n", danio);
        }

    } else if (opcion == 2) {
        // Usar skill
        if (j->numSkills == 0) {
            printf("\nNo tienes skills desbloqueadas todavía.\n");
            return;
        }
        printf("\nSkills disponibles:\n");
        for (int i = 0; i < j->numSkills; i++) {
            printf("  %d. %s — %s\n", i + 1, j->skills[i].nombre, j->skills[i].desc);
        }
        printf("Elige skill (0 para cancelar): ");
        int sk = -1;
        scanf("%d", &sk);
        if (sk >= 1 && sk <= j->numSkills) {
            usarSkill(j, e, sk - 1);
        } else {
            printf("Cancelado.\n");
        }

    } else if (opcion == 3) {
        // Usar item del inventario
        if (j->inv.cantidad == 0) {
            printf("\nNo tienes items.\n");
            return;
        }
        mostrarInventario(j);
        printf("Elige item (0 para cancelar): ");
        int it = -1;
        scanf("%d", &it);
        if (it >= 1 && it <= j->inv.cantidad) {
            usarItem(j, it - 1);
        } else {
            printf("Cancelado.\n");
        }

    } else if (opcion == 4) {
        // Huir: 50% de probabilidad
        int roll = rand() % 100;
        if (roll < 50) {
            printf("\nSalchichín escapó!\n");
            e->hp = -1;  // Señal para terminar combate sin matar al enemigo
        } else {
            printf("\nNo pudo escapar!\n");
        }
    } else {
        printf("\nOpción inválida. Se pierde el turno.\n");
    }
}

// Turno del enemigo: ataca al jugador
void turnoEnemigo(Jugador *j, Enemigo *e) {
    if (e->hp <= 0) return;  // No actúa si está muerto

    if (intentarEsquivar(j->dodge)) {
        printf("\nSalchichín esquivó el ataque de %s!\n", e->nombre);
    } else {
        int danio = calcularDanio(e->dmg);
        j->hp -= danio;
        if (j->hp < 0) j->hp = 0;
        printf("\n%s atacó a Salchichín por %d de daño!\n", e->nombre, danio);
    }
}

// Loop principal del combate por turnos
void iniciarCombate(Jugador *j, Enemigo *e) {
    int huyó = 0;
    printf("\n¡%s apareció!\n", e->nombre);
    _getch();

    while (j->hp > 0 && e->hp > 0) {
        system("cls");
        mostrarEstadoCombate(j, e);
        turnoJugador(j, e);

        if (e->hp == -1) {  // Huyó
            huyó = 1;
            break;
        }

        if (e->hp > 0) {
            turnoEnemigo(j, e);
            printf("Presiona cualquier tecla...\n");
            _getch();
        }
    }

    if (!huyó) {
        if (j->hp <= 0) {
            printf("\nSalchichín fue derrotado...\n");
            _getch();
        } else {
            printf("\n¡Ganaste contra %s!\n", e->nombre);
            generarLoot(j, e);
            ganarXP(j, e->xpReward);
            _getch();
        }
    }
}

// ─────────────────────────────────────────────
// PROGRESIÓN
// ─────────────────────────────────────────────

// Suma XP y verifica si sube de nivel
void ganarXP(Jugador *j, float xpGanada) {
    j->xp += xpGanada;
    printf("\nGanaste %.0f XP! (Total: %.0f)\n", xpGanada, j->xp);

    // Checar subida de nivel
    while (j->nivel < NIVEL_MAX && j->xp >= xpParaNivel[j->nivel]) {
        subirNivel(j);
    }
}

// Sube de nivel y mejora stats
void subirNivel(Jugador *j) {
    j->nivel++;
    j->hpMax += 20;
    j->hp     = j->hpMax;  // Se cura completo al subir de nivel
    j->dmg   += 5;
    j->dodge += 0.05f;

    printf("\n¡¡SUBISTE AL NIVEL %d!!\n", j->nivel);
    printf("  HP Max: %d  |  Daño: %d  |  Esquive: %.0f%%\n",
           j->hpMax, j->dmg, j->dodge * 100);

    desbloquearSkills(j);
}

// Desbloquea skills según el nivel actual
void desbloquearSkills(Jugador *j) {
    for (int i = 0; i < 5; i++) {
        if (j->skills[i].nivelReq == j->nivel) {
            // Mover la skill al arreglo de skills activas
            j->numSkills++;
            printf("\n¡SKILL DESBLOQUEADA: %s!\n", j->skills[i].nombre);
            printf("  %s\n", j->skills[i].desc);
        }
    }
}

// ─────────────────────────────────────────────
// INVENTARIO Y LOOT
// ─────────────────────────────────────────────

// Muestra los items del inventario
void mostrarInventario(Jugador *j) {
    if (j->inv.cantidad == 0) {
        printf("\nInventario vacío.\n");
        return;
    }
    printf("\n--- Inventario ---\n");
    for (int i = 0; i < j->inv.cantidad; i++) {
        char *tipo = j->inv.items[i].tipo == 0 ? "Consumible" :
                     j->inv.items[i].tipo == 1 ? "Arma" : "Armadura";
        printf("  %d. %s [%s] (valor: %d)\n",
               i + 1,
               j->inv.items[i].nombre,
               tipo,
               j->inv.items[i].valor);
    }
}

// Agrega un item al inventario si hay espacio
void agregarItem(Jugador *j, Item it) {
    if (j->inv.cantidad >= 10) {
        printf("\nInventario lleno! No se puede agregar %s.\n", it.nombre);
        return;
    }
    j->inv.items[j->inv.cantidad] = it;
    j->inv.cantidad++;
    printf("\nItem obtenido: %s\n", it.nombre);
}

// Usa un item del inventario
void usarItem(Jugador *j, int indice) {
    if (indice < 0 || indice >= j->inv.cantidad) return;

    Item it = j->inv.items[indice];

    if (it.tipo == 0) {
        // Consumible: cura HP
        j->hp += it.valor;
        if (j->hp > j->hpMax) j->hp = j->hpMax;
        printf("\nUsaste %s. HP restaurado: +%d (HP: %d/%d)\n",
               it.nombre, it.valor, j->hp, j->hpMax);
    } else if (it.tipo == 1) {
        // Arma: aumenta daño permanentemente
        j->dmg += it.valor;
        printf("\nEquipaste %s. Daño aumentado: +%d (Daño total: %d)\n",
               it.nombre, it.valor, j->dmg);
    } else if (it.tipo == 2) {
        // Armadura: aumenta esquive
        j->dodge += 0.05f;
        printf("\nEquipaste %s. Esquive aumentado +5%% (Esquive: %.0f%%)\n",
               it.nombre, j->dodge * 100);
    }

    // Quitar el item usado del inventario
    for (int i = indice; i < j->inv.cantidad - 1; i++) {
        j->inv.items[i] = j->inv.items[i + 1];
    }
    j->inv.cantidad--;
}

// Intenta soltar loot del enemigo derrotado
void generarLoot(Jugador *j, Enemigo *e) {
    int roll = rand() % 100;
    if (roll < e->chanceLoot) {
        printf("\n¡%s soltó: %s!\n", e->nombre, e->lootPosible.nombre);
        agregarItem(j, e->lootPosible);
    }
}

// ─────────────────────────────────────────────
// SKILLS
// ─────────────────────────────────────────────

// Ejecuta la skill elegida
void usarSkill(Jugador *j, Enemigo *e, int indice) {
    if (indice < 0 || indice >= j->numSkills) return;

    Skill sk = j->skills[indice];

    // Costo en HP propio
    if (sk.costoHP > 0) {
        j->hp -= sk.costoHP;
        if (j->hp < 0) j->hp = 0;
        printf("\nUsaste HP propio: -%d\n", sk.costoHP);
    }

    if (sk.dmgExtra > 0) {
        // Skill de daño
        int dmgTotal = calcularDanio(j->dmg + sk.dmgExtra);
        if (intentarEsquivar(e->dodge)) {
            printf("\n%s esquivó %s!\n", e->nombre, sk.nombre);
        } else {
            e->hp -= dmgTotal;
            if (e->hp < 0) e->hp = 0;
            printf("\n%s: Salchichín hizo %d de daño!\n", sk.nombre, dmgTotal);
        }
    } else {
        // Skill sin daño directo (Rodar = esquivar turno, Ladrido = skip enemigo)
        // Rodar: no hace daño, pero el enemigo tampoco atacará este turno
        // Usamos dmgExtra == 0 y costoHP == 0 como señal de skill defensiva
        printf("\nSalchichín usó %s!\n", sk.nombre);
        if (strcmp(sk.nombre, "Rodar") == 0) {
            printf("Salchichín esquivó el próximo ataque!\n");
            j->dodge += 0.50f;  // Boost temporal (se puede resetear después del turno)
        } else if (strcmp(sk.nombre, "Ladrido Feroz") == 0) {
            printf("%s está asustado y pierde su turno!\n", e->nombre);
            e->dmg = 0;  // Pierde el ataque este turno
        }
    }
}

//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon

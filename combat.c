#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include "structures.h"
#include "combat.h"

static float xpParaNivel[] = {0, 100, 250, 450, 700, 1000};
#define NIVEL_MAX 5

typedef struct {
    const char *nombre; int hpMax; float xpReward; int dmg; float dodge; int chanceLoot;
    const char *lootNombre; int lootTipo; int lootValor;
} DatosEnemigo;

static DatosEnemigo datosEnemigos[] = {
    {"Salchicha Normal", 40, 50.0f, 8, 0.05f, 40, "Trozo de Salchicha", 0, 15},
    {"Salchicha Podrida", 70, 100.0f, 14, 0.10f, 55, "Hueso Resistente", 2, 5},
    {"Salchicha Mutante Jefe", 130, 250.0f, 22, 0.15f, 100, "Collar de Campeon", 1, 10}};

void inicializarJugador(Jugador *j) {
    j->hp = j->hpMax = 100; j->xp = 0; j->nivel = 1; j->dmg = 15; j->dodge = 0.10f;
    j->inv.cantidad = j->numSkills = 0;
    Skill todasLasSkills[] = {{"Mordida Fuerte", "Ataque fuerte, +50% dmg", 2, 8, 0},
        {"Rodar", "Esquiva el turno, sin danio", 3, 0, 0},
        {"Ladrido Feroz", "Asusta al enemigo, -1 turno suyo", 4, 0, 0},
        {"Colmillo Rabioso", "Ataque brutal, +100% dmg, -5 HP", 5, 15, 5},
        {"Frenesi Final", "Ataque x2, cuesta 10 HP propios", 5, 20, 10}};
    for (int i = 0; i < 5; i++) j->skills[i] = todasLasSkills[i];
}

Enemigo crearEnemigo(int tipo) {
    Enemigo e; memset(&e, 0, sizeof(Enemigo));
    if (tipo < 0 || tipo > 2) tipo = 0;
    DatosEnemigo d = datosEnemigos[tipo];
    strcpy(e.nombre, d.nombre); e.hp = e.hpMax = d.hpMax; e.xpReward = d.xpReward;
    e.dmg = d.dmg; e.dodge = d.dodge; e.chanceLoot = d.chanceLoot;
    strcpy(e.lootPosible.nombre, d.lootNombre); e.lootPosible.tipo = d.lootTipo;
    e.lootPosible.valor = d.lootValor; return e;
}

void mostrarEstadoCombate(const Jugador *j, const Enemigo *e) {
    printf("\n=============================\n  Salchichin  HP: %d/%d\n  %s  HP: %d/%d\n=============================\n",
           j->hp, j->hpMax, e->nombre, e->hp, e->hpMax);
}

int calcularDanio(int dmgBase) {
    int v = dmgBase / 5; return dmgBase - v + (rand() % (v * 2 + 1));
}
int intentarEsquivar(float dodge) {
    return (float)(rand() % 100) / 100.0f < dodge;
}

void turnoJugador(Jugador *j, Enemigo *e) {
    int opcion;
    printf("\n¿Que hace Salchichin?\n  1. Atacar\n  2. Usar Skill\n  3. Usar Item\n  4. Huir\nOpcion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        if (intentarEsquivar(e->dodge)) printf("\n%s esquivo el ataque!\n", e->nombre);
        else { int d = calcularDanio(j->dmg); if ((e->hp -= d) < 0) e->hp = 0; printf("\nSalchichin ataco por %d de danio!\n", d); }
    }
    else if (opcion == 2) {
        if (j->numSkills == 0) { printf("\nNo tienes skills desbloqueadas.\n"); return; }
        printf("\nSkills disponibles:\n");
        for (int i = 0; i < j->numSkills; i++) printf("  %d. %s — %s\n", i + 1, j->skills[i].nombre, j->skills[i].desc);
        int sk; printf("Elige skill (0 para cancelar): "); scanf("%d", &sk);
        if (sk >= 1 && sk <= j->numSkills) usarSkill(j, e, sk - 1); else printf("Cancelado.\n");
    }
    else if (opcion == 3) {
        if (j->inv.cantidad == 0) { printf("\nNo tienes items.\n"); return; }
        mostrarInventario(j); int it; printf("Elige item (0 para cancelar): "); scanf("%d", &it);
        if (it >= 1 && it <= j->inv.cantidad) usarItem(j, it - 1); else printf("Cancelado.\n");
    }
    else if (opcion == 4) {
        if ((rand() % 100) < 50) { printf("\nSalchichin escapo!\n"); e->hp = -1; }
        else printf("\nNo pudo escapar!\n");
    } else printf("\nOpcion invalida. Turno perdido.\n");
}

void turnoEnemigo(Jugador *j, Enemigo *e) {
    if (e->hp <= 0) return;
    if (intentarEsquivar(j->dodge)) printf("\nSalchichin esquivo el ataque de %s!\n", e->nombre);
    else { int d = calcularDanio(e->dmg); if ((j->hp -= d) < 0) j->hp = 0; printf("\n%s ataco a Salchichin por %d de dano!\n", e->nombre, d); }
}

int iniciarCombate(Jugador *j, Enemigo *e) {
    printf("\n¡%s aparecio!\n", e->nombre); _getch();
    while (j->hp > 0 && e->hp > 0) {
        system("cls"); mostrarEstadoCombate(j, e); turnoJugador(j, e);
        if (e->hp == -1) { _getch(); return -1; }
        if (e->hp > 0) { turnoEnemigo(j, e); printf("Presiona cualquier tecla...\n"); _getch(); }
    }
    if (j->hp <= 0) { printf("\nSalchichin fue derrotado...\n"); _getch(); return 0; }
    printf("\n¡Ganaste contra %s!\n", e->nombre); generarLoot(j, e); ganarXP(j, e->xpReward); _getch(); return 1;
}

// ─────────────────────────────────────────────

void ganarXP(Jugador *j, float xpGanada) {
    j->xp += xpGanada; printf("\nGanaste %.0f XP! (Total: %.0f)\n", xpGanada, j->xp);
    while (j->nivel < NIVEL_MAX && j->xp >= xpParaNivel[j->nivel]) subirNivel(j);
}

void subirNivel(Jugador *j) {
    j->nivel++; j->hpMax += 20; j->hp = j->hpMax; j->dmg += 5; j->dodge += 0.05f;
    printf("\n¡¡SUBISTE AL NIVEL %d!!\n  HP Max: %d  |  Dano: %d  |  Esquive: %.0f%%\n",
           j->nivel, j->hpMax, j->dmg, (int)(j->dodge * 100));
    desbloquearSkills(j);
}

void desbloquearSkills(Jugador *j) {
    for (int i = 0; i < 5; i++)
        if (j->skills[i].nivelReq == j->nivel && j->numSkills <= i) {
            j->numSkills++; printf("\n¡SKILL DESBLOQUEADA: %s!\n  %s\n", j->skills[i].nombre, j->skills[i].desc);
        }
}

void mostrarInventario(const Jugador *j) {
    if (!j->inv.cantidad) { printf("\nInventario vacio.\n"); return; }
    printf("\n--- Inventario ---\n");
    for (int i = 0; i < j->inv.cantidad; i++) {
        const char *t = j->inv.items[i].tipo == 0 ? "Consumible" : j->inv.items[i].tipo == 1 ? "Arma" : "Armadura";
        printf("  %d. %s [%s] (valor: %d)\n", i + 1, j->inv.items[i].nombre, t, j->inv.items[i].valor);
    }
}

void agregarItem(Jugador *j, Item it) {
    if (j->inv.cantidad >= 10) { printf("\nInventario lleno! No se puede agregar %s.\n", it.nombre); return; }
    j->inv.items[j->inv.cantidad] = it; j->inv.cantidad++; printf("\nItem obtenido: %s\n", it.nombre);
}

void usarItem(Jugador *j, int indice) {
    if (indice < 0 || indice >= j->inv.cantidad) return;
    Item it = j->inv.items[indice];
    if (it.tipo == 0) { if ((j->hp += it.valor) > j->hpMax) j->hp = j->hpMax;
        printf("\nUsaste %s. HP: +%d (%d/%d)\n", it.nombre, it.valor, j->hp, j->hpMax); }
    else if (it.tipo == 1) { j->dmg += it.valor; printf("\nEquipaste %s. Dano: +%d (Total: %d)\n", it.nombre, it.valor, j->dmg); }
    else { j->dodge += 0.05f; printf("\nEquipaste %s. Esquive: +5%% (Total: %.0f%%)\n", it.nombre, (int)(j->dodge * 100)); }
    for (int i = indice; i < j->inv.cantidad - 1; i++) j->inv.items[i] = j->inv.items[i + 1];
    j->inv.cantidad--;
}

void generarLoot(Jugador *j, Enemigo *e) {
    if ((rand() % 100) < e->chanceLoot) { printf("\n¡%s solto: %s!\n", e->nombre, e->lootPosible.nombre);
        agregarItem(j, e->lootPosible); }
}

void usarSkill(Jugador *j, Enemigo *e, int indice) {
    if (indice < 0 || indice >= j->numSkills) return;
    Skill sk = j->skills[indice];
    if (sk.costoHP > 0 && j->hp - sk.costoHP <= 0) { printf("\n¡No tienes suficiente HP para usar %s!\n", sk.nombre); return; }
    if (sk.costoHP > 0) j->hp -= sk.costoHP;
    int d = calcularDanio(sk.dmgExtra); if ((e->hp -= d) < 0) e->hp = 0;
    printf("\nSalchichin uso %s por %d de dano!\n", sk.nombre, d);
    if (sk.costoHP > 0) printf("(HP gastate: %d)\n", sk.costoHP);
}

//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon

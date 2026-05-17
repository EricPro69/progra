#ifndef STRUCTURES_H
#define STRUCTURES_H

// Estructura que representa un mapa o pantalla del juego
typedef struct {
    char celdas[20][40]; // Matriz de caracteres que representa el mapa
    int posX;            // Posición X de esta pantalla en el mundo
    int posY;            // Posición Y de esta pantalla en el mundo
} Mapa;

// Estructura del jugador (Salchichín)
struct player
{
    int hp;       // Puntos de vida
    float xp;     // Experiencia
    int dmg;      // Daño del ataque
    float dodge;  // Probabilidad de esquivar
}p1; // Instancia del jugador

// Estructura de enemigos (Salchichas mutadas)
struct enemy{
    int hp;       // Puntos de vida
    float xp;     // Experiencia que otorgan
    int dmg;      // Daño del ataque
    float dodge;  // Probabilidad de esquivar

}en1, en2, en3; // Tres instancias de enemigos

// ─────────────────────────────────────────────
// NUEVAS ESTRUCTURAS — Sistema de combate, inventario y progresión
// ─────────────────────────────────────────────

// Item del inventario
typedef struct {
    char nombre[30];  // Nombre del item
    int tipo;         // 0=consumible, 1=arma, 2=armadura
    int valor;        // Efecto del item (hp que cura, dmg que da, etc.)
} Item;

// Inventario del jugador
typedef struct {
    Item items[10];   // Máximo 10 items
    int cantidad;     // Cuántos items tiene actualmente
} Inventario;

// Skills desbloqueables por nivel
typedef struct {
    char nombre[30];  // Nombre de la skill
    char desc[60];    // Descripción
    int nivelReq;     // Nivel requerido para desbloquear
    int dmgExtra;     // Daño adicional que aplica
    int costoHP;      // Costo en HP propio (0 si no cuesta nada)
} Skill;

// Estado completo del jugador en combate y progresión
typedef struct {
    int hp;           // HP actual
    int hpMax;        // HP máximo
    float xp;         // Experiencia acumulada
    int nivel;        // Nivel actual
    int dmg;          // Daño base
    float dodge;      // Probabilidad de esquivar (0.0 a 1.0)
    Inventario inv;   // Inventario
    Skill skills[5];  // Skills disponibles (máximo 5)
    int numSkills;    // Cuántas skills tiene desbloqueadas
} Jugador;

// Estado de un enemigo en combate
typedef struct {
    char nombre[30];  // Nombre del enemigo
    int hp;           // HP actual
    int hpMax;        // HP máximo
    float xpReward;   // XP que da al morir
    int dmg;          // Daño que hace
    float dodge;      // Probabilidad de esquivar
    // Loot que puede soltar
    Item lootPosible; // Item que puede soltar al morir
    int chanceLoot;   // Probabilidad de soltar loot (0-100)
} Enemigo;

#endif

//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon
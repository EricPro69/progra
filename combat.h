#ifndef COMBAT_H
#define COMBAT_H

#include "structures.h"

// ─────────────────────────────────────────────
// Prototipos — Sistema de combate por turnos
// ─────────────────────────────────────────────

// Inicializacion
void inicializarJugador(Jugador *j);               // Crea al jugador con stats base
Enemigo crearEnemigo(int tipo);                    // Crea un enemigo segun su tipo (0,1,2...)

// Combate
int  iniciarCombate(Jugador *j, Enemigo *e);       // Loop principal del combate por turnos
void turnoJugador(Jugador *j, Enemigo *e);         // El jugador elige accion
void turnoEnemigo(Jugador *j, Enemigo *e);         // El enemigo ataca
int  calcularDanio(int dmgBase);                   // Calcula dano con variacion aleatoria
int  intentarEsquivar(float dodge);                // Retorna 1 si esquiva, 0 si no
void mostrarEstadoCombate(const Jugador *j, const Enemigo *e); // Muestra HP de ambos en pantalla

// Progresion
void ganarXP(Jugador *j, float xpGanada);          // Suma XP y checa si sube de nivel
void subirNivel(Jugador *j);                       // Sube de nivel y mejora stats
void desbloquearSkills(Jugador *j);                // Revisa y desbloquea skills por nivel

// Inventario y loot
void mostrarInventario(const Jugador *j);                // Imprime el inventario
void usarItem(Jugador *j, int indice);             // Usa un item del inventario
void agregarItem(Jugador *j, Item it);             // Anade item al inventario si hay espacio
void generarLoot(Jugador *j, Enemigo *e);          // Intenta soltar loot del enemigo muerto

// Skills
void usarSkill(Jugador *j, Enemigo *e, int indice); // Usa una skill en combate

#endif

//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon

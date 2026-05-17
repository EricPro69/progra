#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "combat.h"

int main(void) {
    Jugador jugador;
    inicializarJugador(&jugador);
    
    // Crear un enemigo para prueba (tipo: 0=Normal, 1=Podrida, 2=Jefe)
    Enemigo enemigo = crearEnemigo(0);  // Normal
    
    printf("=== PRUEBA DE COMBATE ===\n");
    printf("Jugador: HP %d/%d\n", jugador.hp, jugador.hpMax);
    printf("Enemigo: %s HP %d/%d\n\n", enemigo.nombre, enemigo.hp, enemigo.hpMax);
    
    iniciarCombate(&jugador, &enemigo);
    
    printf("\n=== FIN DE COMBATE ===\n");
    printf("HP Final: %d/%d\n", jugador.hp, jugador.hpMax);
    printf("XP Actual: %.0f\n", jugador.xp);
    printf("Nivel: %d\n", jugador.nivel);
    
    return 0;
}

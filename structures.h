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



#endif

//Eric Addai Morales Torres
//Juan Pablo Martinez Romo
//Alejandro Cardoza Salomon
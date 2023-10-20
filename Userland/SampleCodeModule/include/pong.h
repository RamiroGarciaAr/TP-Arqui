#ifndef PONG_H_
#define PONG_H_

#include <utils.h>

#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define PADDLE_HEIGHT 100
#define PADDLE_WIDTH 10
#define SCORE_LIMIT 9
#define BALL_WIDTH 10
#define PADDLE_UP 1
#define PADDLE_DOWN 0

#define ESC 27
#define PLAYER1 0
#define PLAYER2 1

//estructuras usadas en el juego
typedef struct ball_t {
    int x, y;   // posicion en la pantalla
    int w,h;    // ancho (width) y alto (height) de la pelota
    int dx, dy; // vector de movimiento
} ball_t;
typedef ball_t * ball_p;

typedef struct paddle {
    int x,y;
    int w,h;
    int score;
} paddle_t;
typedef paddle_t * paddle_p;

//funcion que inicia el juego
void start_game();

#endif
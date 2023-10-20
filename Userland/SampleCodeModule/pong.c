#include <pong.h>
#include <syscallsManager.h>

uint32_t white = 0xFFFFFF;

paddle_t players[2];
ball_t ball;

static void initNewGame();
static int checkScore();
static void movePaddle(int v, int command);
static int checkCollision(ball_p a, paddle_p b);
static void moveBall();
static void setToDefaultPosition();
static void menuScreen();
static void gameOverScreen(int winner);
static void hasScored(int player);
static void paddleHit(int player);
static void drawFrame();
static int readControls();

static void initNewGame() {
    ball.w = BALL_WIDTH;
    ball.h = BALL_WIDTH;
    ball.dy = 10;
    ball.dx = 10;

    players[0].h = PADDLE_HEIGHT;
    players[0].w = PADDLE_WIDTH;
    players[1].h = PADDLE_HEIGHT;
    players[1].w = PADDLE_WIDTH;
}

void setToDefaultPosition(){
    ball.x = SCREEN_WIDTH / 2;
    ball.y = SCREEN_HEIGHT / 2;
    players[0].x = SCREEN_WIDTH*0.15;
    players[0].y = (SCREEN_HEIGHT/2)-(players[0].h/2);
    players[1].x = SCREEN_WIDTH*0.85;
    players[1].y = (SCREEN_HEIGHT/2)-(players[1].h/2);
}
static void menuScreen(){
    sysDrawCustomCharBack('P',white,SCREEN_WIDTH/2-200,50, 6);
    sysDrawCustomCharBack('O',white,SCREEN_WIDTH/2-100,50, 6);
    sysDrawCustomCharBack('N',white,SCREEN_WIDTH/2,50, 6);
    sysDrawCustomCharBack('G',white,SCREEN_WIDTH/2+100,50, 6);

    sysDrawCustomCharBack('P', white, SCREEN_WIDTH/2-400,400,2);
    sysDrawCustomCharBack('R', white, SCREEN_WIDTH/2-350,400,2);
    sysDrawCustomCharBack('E', white, SCREEN_WIDTH/2-300,400,2);
    sysDrawCustomCharBack('S', white, SCREEN_WIDTH/2-250,400,2);
    sysDrawCustomCharBack('S', white, SCREEN_WIDTH/2-200,400,2);

    sysDrawCustomCharBack('S', white, SCREEN_WIDTH/2+100,400,2);
    sysDrawCustomCharBack('P', white, SCREEN_WIDTH/2+150,400,2);
    sysDrawCustomCharBack('A', white, SCREEN_WIDTH/2+200,400,2);
    sysDrawCustomCharBack('C', white, SCREEN_WIDTH/2+250,400,2);
    sysDrawCustomCharBack('E', white, SCREEN_WIDTH/2+300,400,2);
}

static void gameOverScreen(int winner){
    sysDrawCustomCharBack('G',white,SCREEN_WIDTH/2-200,50, 6);
    sysDrawCustomCharBack('A',white,SCREEN_WIDTH/2-100,50, 6);
    sysDrawCustomCharBack('M',white,SCREEN_WIDTH/2,50, 6);
    sysDrawCustomCharBack('E',white,SCREEN_WIDTH/2+100,50, 6);

    sysDrawCustomCharBack('O',white,SCREEN_WIDTH/2-200,200, 6);
    sysDrawCustomCharBack('V',white,SCREEN_WIDTH/2-100,200, 6);
    sysDrawCustomCharBack('E',white,SCREEN_WIDTH/2,200, 6);
    sysDrawCustomCharBack('R',white,SCREEN_WIDTH/2+100,200, 6);
}

static void drawFrame(){
    sysDrawCustomCharBack(players[1].score+'0',white, SCREEN_WIDTH/2-100,50, 6);
    sysDrawCustomCharBack(players[0].score+'0',white,SCREEN_WIDTH/2+50,50, 6);
    sysDottedLine(white);
    sysDrawFilledRect(white, ball.x, ball.y, ball.w, ball.h);
    //se actualiza cada player en front
    sysDrawFilledRect(white, players[0].x, players[0].y, players[0].w, players[0].h);
    sysDrawFilledRect(white, players[1].x, players[1].y, players[1].w, players[1].h);
    sysVideoRefresh(); //se imprime en pantalla
}

static int readControls(){
    char c;
    sysReadFromBuffer(&c);
    switch(c){
        case 'w':
            movePaddle(PLAYER1, PADDLE_UP);
            break;
        case 's':
            movePaddle(PLAYER1, PADDLE_DOWN);
            break;
        case 'i':
            movePaddle(PLAYER2, PADDLE_UP);
            break;
        case 'k':
            movePaddle(PLAYER2, PADDLE_DOWN);
            break;
        case ESC:
            clearScreen(BACKBUFFER);
            sysVideoRefresh();
            return 0;
    }
    return 1;
}

void start_game(){
    menuScreen();
    sysVideoRefresh();
    char c;
    while((c = getchar())!= ' ');
    initNewGame();
    setToDefaultPosition();
    int cont;
    while(players[0].score <= SCORE_LIMIT && players[1].score <= SCORE_LIMIT){
        //se actualiza la pelota en front
        drawFrame();
        clearScreen(BACKBUFFER);
        cont = readControls();
        if (!cont){
            return;
        }
        moveBall();
    }
    clearScreen(BACKBUFFER);
    gameOverScreen(checkScore());
    sysVideoRefresh();
    char end;
    while ((end = getchar())!= ' ');
    clearScreen(BACKBUFFER);
    sysVideoRefresh();
}


static int checkScore(){
    int winner = 0;

    if(players[0].score == SCORE_LIMIT){
        winner = 1;
    }

    if(players[1].score == SCORE_LIMIT){
        winner = 2;
    }
    //imagino que se resetean los scores por si se quiere volver a jugar sin que salga el juego?
    //si tuviera que salir del programa y volver a entrar ya se inicializarian en 0 automaticamente y no es necesario esto
    if (winner != 0) {
        players[0].score = 0;
        players[1].score = 0;
    }
    return winner;
}

static void movePaddle(int v, int command) {
        if(command == PADDLE_UP){
            if(players[v].y-40 <= 0){
                players[v].y = 0;
            }
            else{
                players[v].y -= 40;
            }
        }
        else{
            if(players[v].y + 40 >= SCREEN_HEIGHT - players[v].h){
                players[v].y = SCREEN_HEIGHT - players[v].h;
            }
            else{
                players[v].y += 40;
            }
        }
        return;
}

//por las dudas lo dejo con punteros para asegurarme que no hay problemas con la estructura recibida
static int checkCollision(ball_p a, paddle_p b) {

    int left_a, left_b;
    int right_a, right_b;
    int top_a, top_b;
    int bottom_a, bottom_b;

    //ball
    left_a = a->x;
    right_a = a->x + a->w;
    top_a = a->y;
    bottom_a = a->y + a->h;

    //players
    left_b = b->x;
    right_b = b->x + b->w;
    top_b = b->y;
    bottom_b = b->y + b->h;

    if (left_a > right_b || right_a < left_b || top_a > bottom_b || bottom_a < top_b) {
        return 0;
    }

    return 1;
}

static void hasScored(int player){
    players[player].score++;
    sysPlayBeep();
    ball.dy= (player == PLAYER1) ? -10 : 10;
    ball.dx= (player == PLAYER1) ? -10 : 10;
    setToDefaultPosition();
}

static void paddleHit(int player){
    if(ball.y<players[player].y+0.33*players[player].h){
        ball.dx= (player == PLAYER1) ? 10 : -10;
        ball.dy= (player == PLAYER1) ? 10 : -10;
    }
    if(ball.y<players[player].y+0.66*players[player].h && ball.y>players[player].y+0.33*players[player].h){
        ball.dy= 0;
        ball.dx= (player == PLAYER1) ? 10 : -10;
    }
    if(ball.y>players[player].y+0.66*players[player].h){
        ball.dx= (player == PLAYER1) ? 10 : -10;
        ball.dy= (player == PLAYER1) ? -10 : -10;
    }
}


static void moveBall(){
    //esto podría ir en un hasScored
    if(ball.x<-10){
        hasScored(PLAYER1);
    }
    if(ball.x>SCREEN_WIDTH-1){
        hasScored(PLAYER2);
    }
    //esto sería para paddleHit
    if(checkCollision(&ball, &players[PLAYER1])){
        paddleHit(PLAYER1);
    }

    if(checkCollision(&ball, &players[PLAYER2])){
        paddleHit(PLAYER2);
    }

    //esto sería wallHit
    if(ball.y<1+BALL_WIDTH ||ball.y>SCREEN_HEIGHT-1){
        ball.dy=-ball.dy;
    }
    //esto seria el move Ball concreto
    ball.x+=ball.dx;
    ball.y+=ball.dy;
    return;
}
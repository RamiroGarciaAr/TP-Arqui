#include <player.h>
#include <snake.h>
#include <syscallsManager.h>
#include <point.h>
#include <stdlib.h>
#include <time.h>
#include <apple.h>
#include <color.h>
#include <utils.h>

//Color white = {0xFF,0xFF,0xFF};

//Colors
uint32_t whiteColor = 0xFFFFFF;
uint32_t redColor = 0xFF0000;
uint32_t blackColor = 0x000000; 

Apple apple;
Player player;


//static void gameOverScreen();
static void menuScreen();
static void drawFrame();
AxisPoint getRandomPoint();
static void setup();

void start_snake(){
    menuScreen();
    sysVideoRefresh();

    char c;
    while((c = getchar()) != ' ');

    setup();
    
    clearScreen(BACKBUFFER);
    sysVideoRefresh();

}

static void setup()
{

    //Player set up
    player.points = 0;
    player.isAlive = 1;
    player.playerPos.x = SCREEN_WIDTH/2;
    player.playerPos.y = SCREEN_HEIGHT/2;
    player.moveDir = 3; // <--
    player.currentSize = PLAYER_INIT_SIZE;
    player.height = DOT_SIZE;
    player.color = whiteColor;

    //Apple setup
    apple.size = DOT_SIZE;
    apple.color = redColor;
    apple.applePos.x = getRandom(SCREEN_WIDTH,0);
    apple.applePos.y = getRandom(SCREEN_HEIGHT,0);
    drawFrame();
}

static int readControls()
{
    char c;
    sysReadFromBuffer(&c);
    switch (c)
    {
        case 'w':
            moveUp();
            break;
        case 's'
            moveDown();
            break;
        case 'd':
            moveRight();
            break;
        case 'a':
            moveLeft();
            break;
        case ESC:
            clearScreen(BACKBUFFER);
            sysVideoRefresh();
            return 0;
    }
}


//static void moveUp(){
//    int isValid = checkPixel()
//
//
//
//}

//hay q cambiar como definimos la estructura del color??
//si lo queremos hacer como el tron, va a ser un tema


static void menuScreen()
{
    sysDrawCustomCharBack('S',whiteColor,SCREEN_WIDTH/2-200,50, 6);
    sysDrawCustomCharBack('N',whiteColor,SCREEN_WIDTH/2-100,50, 6);
    sysDrawCustomCharBack('A',whiteColor,SCREEN_WIDTH/2,50, 6);
    sysDrawCustomCharBack('K',whiteColor,SCREEN_WIDTH/2+100,50, 6);
    sysDrawCustomCharBack('E',whiteColor,SCREEN_WIDTH/2+200,50, 6);

    sysDrawCustomCharBack('P', whiteColor, SCREEN_WIDTH/2-400,400,2);
    sysDrawCustomCharBack('R', whiteColor, SCREEN_WIDTH/2-350,400,2);
    sysDrawCustomCharBack('E', whiteColor, SCREEN_WIDTH/2-300,400,2);
    sysDrawCustomCharBack('S', whiteColor, SCREEN_WIDTH/2-250,400,2);
    sysDrawCustomCharBack('S', whiteColor, SCREEN_WIDTH/2-200,400,2);

    sysDrawCustomCharBack('S', whiteColor, SCREEN_WIDTH/2+100,400,2);
    sysDrawCustomCharBack('P', whiteColor, SCREEN_WIDTH/2+150,400,2);
    sysDrawCustomCharBack('A', whiteColor, SCREEN_WIDTH/2+200,400,2);
    sysDrawCustomCharBack('C', whiteColor, SCREEN_WIDTH/2+250,400,2);
    sysDrawCustomCharBack('E', whiteColor, SCREEN_WIDTH/2+300,400,2);
}

static void drawFrame()
{
    sysDrawCustomCharBack(player.points +'0',whiteColor, SCREEN_WIDTH/2,50, 6);
    sysDrawFilledRect(whiteColor,apple.applePos.x, apple.applePos.x, apple.size, apple.size);
    //se actualiza cada player en front
    sysDrawFilledRect(whiteColor, player.playerPos.x, player.playerPos.y, player.currentSize, player.height);
    sysVideoRefresh(); //se imprime en pantalla
}

/*
static void gameOverScreen()
{
    sysDrawCustomCharBack('G',whiteColor,SCREEN_WIDTH/2-200,50, 6);
    sysDrawCustomCharBack('A',whiteColor,SCREEN_WIDTH/2-100,50, 6);
    sysDrawCustomCharBack('M',whiteColor,SCREEN_WIDTH/2,50, 6);
    sysDrawCustomCharBack('E',whiteColor,SCREEN_WIDTH/2+100,50, 6);

    sysDrawCustomCharBack('O',whiteColor,SCREEN_WIDTH/2-200,200, 6);
    sysDrawCustomCharBack('V',whiteColor,SCREEN_WIDTH/2-100,200, 6);
    sysDrawCustomCharBack('E',whiteColor,SCREEN_WIDTH/2,200, 6);
    sysDrawCustomCharBack('R',whiteColor,SCREEN_WIDTH/2+100,200, 6);
}
*/
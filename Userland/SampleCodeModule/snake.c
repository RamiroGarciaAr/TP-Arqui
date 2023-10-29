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
uint32_t blueColor=	0x0000FF;

Apple apple;
Player player;


//static void gameOverScreen();
static void menuScreen();
static void drawFrame();
AxisPoint getRandomPoint();
static void setup();

void start_snake(){
    //Beginning the game
    menuScreen();
    sysVideoRefresh();

    char c;
    while((c = getchar()) != ' ');

    setup();
    
    clearScreen(BACKBUFFER);
    sysVideoRefresh();


    //Finishing the game
    gameOverScreen(checkScore());
    sysVideoRefresh();
    char end;
    while ((end = getchar())!= ' ');
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
            if (player.moveDir != 2)
                moveUp();
            break;
        case 's':
            if (player.moveDir != 1)
                moveDown();
            break;
        case 'd':
            if (player.moveDir != 3)
                moveRight();
            break;
        case 'a':
            if (player.moveDir != 4)
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
    sysDrawFilledRect(redColor,apple.applePos.x, apple.applePos.x, apple.size, apple.size);
    //se actualiza cada player en front
    //sysDrawFilledRect(whiteColor, player.playerPos.x, player.playerPos.y, player.currentSize, player.height);
    drawSnake();
    sysVideoRefresh(); //se imprime en pantalla
}
static void drawSnake()
{
    for (int i =0;i<player.currentSize;i++)
    {
        sysDrawFilledRect(whiteColor, player.playerPos[i].x, player.playerPos[i].y, DOT_SIZE, DOT_SIZE);
    }
}
static void deleteLast()
{
    sysDrawFilledRect(whiteColor, player.playerPos[player.currentSize].x, player.playerPos[player.currentSize].y, DOT_SIZE, DOT_SIZE);
}
//checks the color of the nextPixel
//returns -1 if crashed 0 if all good :) 
//
static int validateMovement(int posX, int posY)
{
    
}


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
void changeSnakeSegmentsPos()
{
    for(i=player.currentSize;i<1;i--)
    {
      array[i]=array[i-1];
            
    }
    
}

void moveUp()
{
    if(validateMovement(player.playerPos[i].x, player.playerPos[i].y + DOT_SIZE) == -1)
    {
        gameOverScreen();
    }
    changeSnakeSegmentsPos();
    player.playerPos[0].y=player.playerPos[0].y + DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
    
    player.moveDir = 1;

}
void moveDown()
{
    if(validateMovement(player.playerPos[i].x, player.playerPos[i].y + DOT_SIZE) == -1){
        gameOverScreen();
    }
     changeSnakeSegmentsPos();
     player.playerPos[0].y=player.playerPos[0].y - DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
   

    player.moveDir = 2;
}
void moveleft()
{
    if(validateMovement(player.playerPos[i].x - DOT_SIZE, player.playerPos[i].y ) == -1){
        gameOverScreen();
    }
    changeSnakeSegmentsPos();
    player.playerPos[0].x=player.playerPos[0].x - DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
    

    player.movDir = 3;
}
void moveRight()
{
    if(validateMovement(player.playerPos[0].x + DOT_SIZE, player.playerPos[0].y ) == -1){
        gameOverScreen();
    }
    changeSnakeSegmentsPos(); 
    player.playerPos[0].x=player.playerPos[0].x  + DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
    
    
    player.movDir = 4;
}
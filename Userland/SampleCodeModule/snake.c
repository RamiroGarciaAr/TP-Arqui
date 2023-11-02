#include <snake.h>

#define DOT  0x000001
#define DEF_HEIGHT 16
#define DEF_WIDTH 9
//Color white = {0xFF,0xFF,0xFF};

//Colors
uint32_t whiteColor = 0xFFFFFF;
uint32_t redColor = 0xFF0000;
uint32_t blackColor = 0x000000; 
uint32_t blueColor=	0x0000FF;

Apple apple;
Player player;

void start_snake();
static void gameOverScreen();
static void menuScreen();
static void drawFrame();
static AxisPoint getRandomPoint();
static void setup();
static void readControls();
static void drawSnake();
static void deleteLast();
static void changeSnakeSegmentsPos();
static void moveUp();
static void moveDown();
static void moveLeft();
static void moveRight();
static void eatApple();
void checkCollision(int movDir);
static void setupPlayerPosition();
static void borders();



static void setup()
{
    //Player set up
    player.points = 0;
    player.isAlive = 1;
    player.playerPos[0].x = SCREEN_WIDTH/2;
    player.playerPos[0].y = SCREEN_HEIGHT/2;
    player.moveDir = 3; // <--
    player.currentSize = PLAYER_INIT_SIZE;
    setupPlayerPosition();
    player.color = whiteColor;

    //Apple setup
    apple.size = DOT_SIZE;
    apple.color = redColor;
    apple.applePos = getRandomPoint();
    borders();
}

static void setupPlayerPosition()
{
    for(int i=0;i<player.currentSize;i++)
    {
        player.playerPos[i].x = SCREEN_WIDTH/2 + DOT_SIZE*i;
        player.playerPos[i].y = SCREEN_HEIGHT/2;
    }
}

//Reads Player Inputs
static void readControls()
{
    char c;
    sysReadFromBuffer(&c);
    switch (c)
    {
        case 'w':
            if (player.moveDir != 2 || player.moveDir != 1)
                player.moveDir = 1;
            break;
        case 's':
            if (player.moveDir != 1 || player.moveDir != 2)
                player.moveDir = 2;
            break;
        case 'd':
            if (player.moveDir != 3 || player.moveDir != 4 )
                player.moveDir = 4;
            break;
        case 'a':
            if (player.moveDir != 4 || player.moveDir != 3)
                player.moveDir = 3;
            break;
        case ESC:
            clearScreen(BACKBUFFER);
            player.isAlive =0;
            sysVideoRefresh();
            return;
    }
}


static void playerMovement()
{
    int moveDir = player.moveDir;
    switch (moveDir)
    {
    case 1:
        moveUp();
        break;
    case 2:
        moveDown();
        break;
    case 3:
        moveLeft();
        break;
    case 4:
        moveRight();
        break;
    default:
        break;
    }
}
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

static void borders(){
    uint16_t scrW,scrY;
    sysGetScrWidth(&scrW);
    getScreenHeight(&scrY);
    uint16_t xLimit = scrW -DEF_WIDTH;
    uint16_t yLimit = scrY -DEF_HEIGHT;
    uint16_t x = DEF_WIDTH;
    uint16_t y = DEF_HEIGHT;

    // Dibuja los limites de la pantalla
    sysDrawFilledRect(whiteColor,x,y,xLimit,DOT_SIZE);
    sysDrawFilledRect(whiteColor,x,y,DOT_SIZE,yLimit);
    sysDrawFilledRect(whiteColor,x,760,xLimit,DOT_SIZE);
    sysDrawFilledRect(whiteColor,1015,y,DOT_SIZE,yLimit);
}


//draws game status
static void drawFrame()
{
    sysDrawCustomCharBack(player.points +'0',whiteColor, SCREEN_WIDTH/2,50, 6);
    sysDrawFilledRect(redColor,apple.applePos.x, apple.applePos.y, apple.size, apple.size);
    drawSnake();
    borders();
    sysVideoRefresh(); //se imprime en pantalla
}


static void drawSnake()
{
    //Subject to change
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
//returns if crashed 0 if all good :) 
//
static uint32_t validate(uint64_t x, uint64_t y)
{
    uint32_t c;
    sysGetPtrToPixel(x, y, &c);
    return c;
}

uint32_t check(uint32_t color)
{
    return blackColor - color;
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
static void changeSnakeSegmentsPos()
{
    for(int i=player.currentSize;i<1;i--)
    {
        player.playerPos[i]=player.playerPos[i-1];
    }
    
}

static void moveUp()
{
    if (check(validate(player.playerPos[0].x,player.playerPos[0].y+DOT)) == 0)
    {
        player.playerPos[0].y=player.playerPos[0].y - DOT_SIZE;
        sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    }
    else    
    {
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 
}
static void moveDown()
{
    if (check(validate(player.playerPos[0].x,player.playerPos[0].y-DOT)) == 0)
    {
        player.playerPos[0].y=player.playerPos[0].y + DOT_SIZE;
        sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    }
    else
    {
            clearScreen(BACKBUFFER);
            player.isAlive =0;
            sysVideoRefresh();
    } 
}
static void moveLeft()
{
    if (check(validate(player.playerPos[0].x - DOT,player.playerPos[0].y)) == 0)
    {
        player.playerPos[0].x=player.playerPos[0].x - DOT_SIZE;
        sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    }
    else    
    {
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 

}
static void moveRight()
{
    if (check(validate(player.playerPos[0].x + DOT,player.playerPos[0].y)) == 0)
    {
        player.playerPos[0].x=player.playerPos[0].x  + DOT_SIZE;
        sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    }
    else    
    {
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 

}

static AxisPoint getRandomPoint()
{
    AxisPoint point;
    
    point.x = getRandomInBetween(SCREEN_WIDTH,0);
    point.y = getRandomInBetween(SCREEN_HEIGHT,0);

    return point;
}

void eatApple()
{
    player.currentSize++;
    sysDrawFilledRect(whiteColor,  apple.applePos.x ,  apple.applePos.y, DOT_SIZE, DOT_SIZE);
    changeSnakeSegmentsPos();
    player.playerPos[0].x=apple.applePos.x;
    player.playerPos[0].y=apple.applePos.y;

    apple.applePos = getRandomPoint();
}

void start_snake(){
    //Beginning the game
    menuScreen();
    sysVideoRefresh();
    char c;
    while((c = getchar()) != ' ');
    setup();

    //EXECUTE GAME
    while (player.isAlive != 0)
    {
        drawFrame();
        clearScreen(BACKBUFFER);
        readControls();
        playerMovement();
    }
    
    if(player.isAlive == 0){
        clearScreen(BACKBUFFER);
        //YOU LOST :C
        gameOverScreen();
        sysVideoRefresh();

        char end;
        while ((end = getchar())!= ' ');
        clearScreen(BACKBUFFER);
        sysVideoRefresh();
    }
}

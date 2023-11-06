#include <snake.h>
#include <syscallsManager.h>

#define DOT  0x000001
#define DEF_HEIGHT 16
#define DEF_WIDTH 9
//Color white = {0xFF,0xFF,0xFF};

//Colors
uint64_t whiteColor = 0xFFFFFF;
uint64_t redColor = 0xFF0000;
uint64_t blackColor = 0x000000; 
uint64_t blueColor =  0x0000FF;
uint64_t grayColor = 0x808080;

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
static void moveUp();
static void moveDown();
static void moveLeft();
static void moveRight();
static void eatApple();
void checkCollision(int movDir);
static void borders();

static void setPlayerPosition(uint64_t startX,uint64_t startY)
{
    player.playerPos[0].x = startX;
    player.playerPos[0].y = startY;

    for (int i=1;i<player.currentSize;i++)
    {
        player.playerPos[i].x = startX+DOT_SIZE*i;
        player.playerPos[i].y = startY;
    }
}

static void setup()
{
    //Player set up
    player.points = 0;
    player.isAlive = 1;

    player.moveDir = 3; // <--
    player.currentSize = PLAYER_INIT_SIZE;

    setPlayerPosition(SCREEN_WIDTH/2,SCREEN_HEIGHT/2);

    player.color = whiteColor;

    //Apple setup
    apple.size = DOT_SIZE;
    apple.color = redColor;
    apple.applePos = getRandomPoint();
    borders();
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
            sysPlayBeep();
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
    scrW = sysGetScrWidth();
    scrY = sysGetScrHeight();

    // Dibuja los limites de la pantalla
    sysDrawFilledRect(whiteColor,0,scrY - DOT_SIZE,scrW,DOT_SIZE); //Abajo
    sysDrawFilledRect(whiteColor,0,0,scrW,DOT_SIZE); //Arriba
    sysDrawFilledRect(whiteColor,0,0,DOT_SIZE,scrY); //Izq
    sysDrawFilledRect(whiteColor,scrW-DOT_SIZE,0,DOT_SIZE,scrY); //Right
}


//draws game status
static void drawFrame()
{
    sysDrawCustomCharBack(player.points +'0',grayColor, SCREEN_WIDTH/2,50, 6);
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
        sysDrawFilledRect(whiteColor, player.playerPos[i].x - DOT_SIZE / 2, player.playerPos[i].y - DOT_SIZE / 2, DOT_SIZE, DOT_SIZE);
    }
}

//checks the color of the nextPixel
//returns if crashed 0, if not, all good :) 
//
static uint64_t validate(uint64_t x, uint64_t y)
{
    uint64_t c;
    sysGetPtrToPixel(x, y, &c);
    return c;
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
static void updatePlayerTail(AxisPoint lastPosition)
{
    player.playerPos[1] = lastPosition;
    for (int i=player.currentSize-1;i>1;i--)
    {
        player.playerPos[i] = player.playerPos[i-1];
    }

}

static void moveUp()
{
    if (validate(player.playerPos[0].x,player.playerPos[0].y-DOT_SIZE) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].y=player.playerPos[0].y - DOT_SIZE;
        updatePlayerTail(lastPostion);

    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 
}
static void moveDown()
{
    if (validate(player.playerPos[0].x,player.playerPos[0].y+DOT_SIZE) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].y=player.playerPos[0].y + DOT_SIZE;
        updatePlayerTail(lastPostion);
    }
    else
    {
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player.isAlive =0;
            sysVideoRefresh();
    } 
}
static void moveLeft()
{
    if (validate(player.playerPos[0].x - DOT_SIZE,player.playerPos[0].y) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].x=player.playerPos[0].x - DOT_SIZE;
        updatePlayerTail(lastPostion);
    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 

}
static void moveRight()
{
    if (validate(player.playerPos[0].x + DOT_SIZE,player.playerPos[0].y) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].x=player.playerPos[0].x  + DOT_SIZE;
        updatePlayerTail(lastPostion);
    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 

}

static AxisPoint getRandomPoint()
{
    AxisPoint point;
    
    point.x = getRandomInBetween(sysGetScrWidth()- DOT_SIZE,DOT_SIZE);
    point.y = getRandomInBetween(sysGetScrHeight() - DOT_SIZE,DOT_SIZE);

    return point;
}

static void eatApple()
{
    player.currentSize++;
    apple.applePos = getRandomPoint();
    //Increse Size
    player.playerPos[player.currentSize].x = player.playerPos[player.currentSize-1].x + DOT_SIZE;
    player.playerPos[player.currentSize].y = player.playerPos[player.currentSize-1].y;

}


static void increase_score()
{
    if(validate(player.playerPos[0].x,player.playerPos[0].y) == redColor){
        eatApple();
        player.points++;
        drawSnake();
    }
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
        increase_score();
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

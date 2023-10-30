#include <snake.h>
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
static int readControls();
static void defineEdges();
static void drawSnake();
static void deleteLast();
static int validateMovement(uint64_t posX, uint64_t posY);
static void changeSnakeSegmentsPos();
static void moveUp();
static void moveDown();
static void moveLeft();
static void moveRight();
static void move();
static void eatApple();


void start_snake(){
    //Beginning the game
    menuScreen();
    sysVideoRefresh();

    char c;
    while((c = getchar()) != ' ');
    setup();
    int input;
    //EXECUTE GAME
    while (player.isAlive)
    {
        drawFrame();
        clearScreen(BACKBUFFER);
        input = readControls();
        move();
        if(!input)
        {
            return;
        }
    }
    clearScreen(BACKBUFFER);
    //YOU LOST :C
    gameOverScreen();
    sysVideoRefresh();


    sysVideoRefresh();
    char end;
    while ((end = getchar())!= ' ');
    clearScreen(BACKBUFFER);
    drawFrame();
    
    sysVideoRefresh();
}

static void setup()
{
    //Player set up
    player.points = 0;
    player.isAlive = 1;
    player.playerPos[0].x = SCREEN_WIDTH/2;
    player.playerPos[0].y = SCREEN_HEIGHT/2;
    player.moveDir = 3; // <--
    player.currentSize = PLAYER_INIT_SIZE;
    player.color = whiteColor;

    //Apple setup
    apple.size = DOT_SIZE;
    apple.color = redColor;
    apple.applePos = getRandomPoint();
    defineEdges();
}

//Reads Player Inputs
static int readControls()
{
    char c;
    sysReadFromBuffer(&c);
    switch (c)
    {
        case 'w':
            if (player.moveDir != 2)
                moveUp();
                return 1;
            break;
        case 's':
            if (player.moveDir != 1)
                moveDown();
                return 2;
            break;
        case 'd':
            if (player.moveDir != 3)
                moveRight();
                return 4;
            break;
        case 'a':
            if (player.moveDir != 4)
                moveLeft();
                return 3;
            break;
        case ESC:
            clearScreen(BACKBUFFER);
            sysVideoRefresh();
            return 0;
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
static void defineEdges()
{
    sysDrawFilledRect(whiteColor,0,0,DOT_SIZE,SCREEN_HEIGHT); //
    sysDrawFilledRect(whiteColor,SCREEN_WIDTH,0,DOT_SIZE,SCREEN_HEIGHT);
    sysDrawFilledRect(whiteColor,0,0,SCREEN_WIDTH,DOT_SIZE);
    sysDrawFilledRect(whiteColor,0,SCREEN_HEIGHT,SCREEN_WIDTH,DOT_SIZE);
}

//draws game status
static void drawFrame()
{
    sysDrawCustomCharBack(player.points +'0',whiteColor, SCREEN_WIDTH/2,50, 6);
    sysDrawFilledRect(redColor,apple.applePos.x, apple.applePos.y, apple.size, apple.size);
    drawSnake();
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
static int validateMovement(uint64_t posX, uint64_t posY)
{
    uint32_t nextPixelColor;
    sysGetPtrToPixel(posX,posY,&nextPixelColor);
    if ((nextPixelColor - redColor) == 0) 
    {
        eatApple();
        return 0;
    }
    return nextPixelColor - blackColor;
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
    if(validateMovement(player.playerPos[0].x, player.playerPos[0].y + DOT_SIZE) != 0)
        player.isAlive = 0;
    changeSnakeSegmentsPos();
    player.playerPos[0].y=player.playerPos[0].y + DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
    
    player.moveDir = 1;

}
static void moveDown()
{
    if(validateMovement(player.playerPos[0].x, player.playerPos[0].y + DOT_SIZE) != 0)
        player.isAlive = 0;
    changeSnakeSegmentsPos();
    player.playerPos[0].y=player.playerPos[0].y - DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
   

    player.moveDir = 2;
}
static void moveLeft()
{
    if(validateMovement(player.playerPos[0].x - DOT_SIZE, player.playerPos[0].y ) != -0)
        player.isAlive = 0;
    changeSnakeSegmentsPos();
    player.playerPos[0].x=player.playerPos[0].x - DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
    

    player.moveDir = 3;
}
static void moveRight()
{
    if(validateMovement(player.playerPos[0].x + DOT_SIZE, player.playerPos[0].y ) != 0)
        player.isAlive = 0;
    changeSnakeSegmentsPos(); 
    player.playerPos[0].x=player.playerPos[0].x  + DOT_SIZE;
    sysDrawFilledRect(whiteColor, player.playerPos[0].x, player.playerPos[0].y, DOT_SIZE, DOT_SIZE);
    deleteLast();
    
    
    player.moveDir = 4;
}

static AxisPoint getRandomPoint()
{
    AxisPoint point;
    
    point.x = getRandomInBetween(SCREEN_WIDTH,0);
    point.y = getRandomInBetween(SCREEN_HEIGHT,0);

    return point;
}

static void move(){
    if(player.moveDir==1){
        moveUp();
    }
    if(player.moveDir==2){
        moveDown();
    }
    if(player.moveDir==3){
        moveLeft();
    }
     if(player.moveDir==4){
        moveRight();
     }

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

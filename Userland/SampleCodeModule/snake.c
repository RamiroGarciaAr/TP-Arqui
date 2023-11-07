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
Player player[2];

void start_snake();
static void gameOverScreen();
static void menuScreen();
static void drawFrame(int mode);
static AxisPoint getRandomPoint();
static void setup(int mode);
static void readControls();
static void drawSnake(int v);

static void moveUp(int v);
static void moveDown(int v);
static void moveLeft(int v);
static void moveRight(int v);
static void eatApple(int v);
void checkCollision(int movDir);
static void borders();
static void readControls2();

int lastmoveDir1;
int lastmoveDir2;

static void setPlayerPosition(uint64_t startX,uint64_t startY,int v)
{
    player[v].playerPos[0].x = startX;
    player[v].playerPos[0].y = startY;

    for (int i=1;i<player[v].currentSize;i++)
    {
        player[v].playerPos[i].x = startX+DOT_SIZE*i;
        player[v].playerPos[i].y = startY;
    }
}

static void setup(int mode)
{
    //Player set up
    player[0].points = 0;
    player[0].isAlive = 1;
    player[0].color = whiteColor;
    player[0].moveDir = 3; // <--
    player[0].currentSize = PLAYER_INIT_SIZE;

    if(mode==2){
    player[1].points = 0;
    player[1].isAlive = 1;
    player[1].color = whiteColor;
    player[1].moveDir = 3; // <--
    player[1].currentSize = PLAYER_INIT_SIZE;
    setPlayerPosition(SCREEN_WIDTH*0.25,SCREEN_HEIGHT/2,0);
    setPlayerPosition(SCREEN_WIDTH*0.75,SCREEN_HEIGHT/2,1);
    }

    
    if(mode==1){
    setPlayerPosition(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,0);}


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
    lastmoveDir1 = player[0].moveDir;
    switch (c)
    {
        case 'w':
            if (player[0].moveDir != 2)
            player[0].moveDir = 1;
            break;
        case 's':
            if (player[0].moveDir != 1)
                player[0].moveDir = 2;
            break;
        case 'd':
            if (player[0].moveDir != 3)
                player[0].moveDir = 4;
            break;
        case 'a':
            if (player[0].moveDir != 4)
                player[0].moveDir = 3;
            break;
        case ESC:
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player[0].isAlive =0;
            sysVideoRefresh();
            return;
    }
}

static void readControls2()
{
    char c;
    sysReadFromBuffer(&c);
    lastmoveDir2 = player[1].moveDir;
    switch (c)
    {
        case 'i':
            if (player[1].moveDir != 2)
            player[1].moveDir = 1;
            break;
        case 'k':
            if (player[1].moveDir != 1)
                player[1].moveDir = 2;
            break;
        case 'l':
            if (player[1].moveDir != 3)
                player[1].moveDir = 4;
            break;
        case 'j':
            if (player[1].moveDir != 4)
                player[1].moveDir = 3;
            break;
        case ESC:
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player[1].isAlive =0;
            sysVideoRefresh();
            return;
    }
}


static void playerMovement(int v,int mode)
{
    int moveDir = player[v].moveDir;
    int lastmoveDir;
    if(mode==1){
         lastmoveDir = lastmoveDir1;
    }
    else{
         lastmoveDir = lastmoveDir2;
    }
    switch (moveDir)
    {
    case 1:
        if (lastmoveDir != 2)
            moveUp(v);
        break;
    case 2:
        if (lastmoveDir != 1)
            moveDown(v);
        break;
    case 3:
        if (lastmoveDir != 4)
            moveLeft(v);
        break;
    case 4:
        if (lastmoveDir != 3)
            moveRight(v);
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
static void drawFrame(int mode)
{
    if(mode==1){
    sysDrawCustomNumber(player[0].points,grayColor, SCREEN_WIDTH/2,50, 6);}
    if(mode==2){
        sysDrawCustomNumber(player[0].points,grayColor, SCREEN_WIDTH*0.25,50, 6);
        sysDrawCustomNumber(player[1].points,grayColor, SCREEN_WIDTH*0.75,50, 6);
        drawSnake(1);
    }
    
        
    
    sysDrawFilledRect(redColor,apple.applePos.x, apple.applePos.y, apple.size, apple.size);
    
    drawSnake(0);
    
    borders();
    sysVideoRefresh(); //se imprime en pantalla
}


static void drawSnake(int v)
{
    //Subject to change
    for (int i =0;i<player[v].currentSize;i++)
    {
        sysDrawFilledRect(whiteColor, player[v].playerPos[i].x - DOT_SIZE / 2, player[v].playerPos[i].y - DOT_SIZE / 2, DOT_SIZE, DOT_SIZE);
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
static void updatePlayerTail(AxisPoint lastPosition,int v)
{
    player[v].playerPos[1] = lastPosition;
    for (int i=player[v].currentSize-1;i>1;i--)
    {
        player[v].playerPos[i] = player[v].playerPos[i-1];
    }

}

static void moveUp(int v)
{
    if (validate(player[v].playerPos[0].x,player[v].playerPos[0].y-DOT_SIZE) != whiteColor)
    {
        AxisPoint lastPostion = player[v].playerPos[0];
        player[v].playerPos[0].y=player[v].playerPos[0].y - DOT_SIZE;
        updatePlayerTail(lastPostion,v);

    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player[v].isAlive =0;
        sysVideoRefresh();
    } 
}
static void moveDown(int v)
{
    if (validate(player[v].playerPos[0].x,player[v].playerPos[0].y+DOT_SIZE) != whiteColor)
    {
        AxisPoint lastPostion = player[v].playerPos[0];
        player[v].playerPos[0].y=player[v].playerPos[0].y + DOT_SIZE;
        updatePlayerTail(lastPostion,v);
    }
    else
    {
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player[v].isAlive =0;
            sysVideoRefresh();
    } 
}
static void moveLeft(int v)
{
    if (validate(player[v].playerPos[0].x - DOT_SIZE,player[v].playerPos[0].y) != whiteColor)
    {
        AxisPoint lastPostion = player[v].playerPos[0];
        player[v].playerPos[0].x=player[v].playerPos[0].x - DOT_SIZE;
        updatePlayerTail(lastPostion,v);
    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player[v].isAlive =0;
        sysVideoRefresh();
    } 

}
static void moveRight(int v)
{
    if (validate(player[v].playerPos[0].x + DOT_SIZE,player[v].playerPos[0].y) != whiteColor)
    {
        AxisPoint lastPostion = player[v].playerPos[0];
        player[v].playerPos[0].x=player[v].playerPos[0].x  + DOT_SIZE;
        updatePlayerTail(lastPostion,v);
    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player[v].isAlive =0;
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

static void eatApple(int v)
{
    player[v].currentSize++;
    apple.applePos = getRandomPoint();
    //Increse Size
    player[v].playerPos[player[v].currentSize].x = player[v].playerPos[player[v].currentSize-1].x + DOT_SIZE;
    player[v].playerPos[player[v].currentSize].y = player[v].playerPos[player[v].currentSize-1].y;

}


static void increase_score(int v)
{
    if(validate(player[v].playerPos[0].x,player[v].playerPos[0].y) == redColor){
        eatApple(v);
        player[v].points++;
        drawSnake(v);
    }
}

void drawString(char *str,uint64_t color,int spacing,int x,int y,int size)
{
    for (int i=0;i<strlen(str);i++)
    {
        sysDrawCustomCharBack(str[i],color,x+(spacing*i),y, size);
    }
}

void start_snake(){
    //Beginning the game
    menuScreen();
    sysVideoRefresh();
    char c;
    while((c = getchar()) != ' ');
    
    //EXECUTE MENU
    while((c = getchar()) != '1')
    {
        c = getchar();
        clearScreen(BACKBUFFER);
        //drawGamemodeMenu();
        drawString("SELECT",whiteColor,25,300,200,3);
        drawString("GAMEMODE",whiteColor,25,300,250,3);

        drawString("1-Singleplayer",whiteColor,25,300,350,2);
        drawString("2-Competitive",whiteColor,25,300,400,2);
        sysVideoRefresh();

    }
   
    //EXECUTE GAME
    // if (c == '1')
    // {
    //      setup(1);
    //     while (player[0].isAlive != 0)
    //     {
    //         drawFrame(1,0);
    //         clearScreen(BACKBUFFER);
    //         readControls();
    //         playerMovement(0,1);
    //         increase_score(0);
    //     }
    // }
    // if (c == '2')
    // {
        setup(2);
          while (player[0].isAlive != 0 && player[1].isAlive!=0 )
        {
            drawFrame(2);
            clearScreen(BACKBUFFER);
            readControls();
            playerMovement(0,1);
            increase_score(0);

            
            
            readControls2();
            playerMovement(1,2);
            increase_score(1);
        }
        
    //}

    
    if(player[0].isAlive == 0|| player[1].isAlive == 0){
        clearScreen(BACKBUFFER);
        //YOU LOST :C
        //gameOverScreen(); sysDrawCustomCharBack('G',whiteColor,SCREEN_WIDTH/2-200,50, 6);
        drawString("GAME",whiteColor,50,SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2 - 200,6);
        drawString("OVER",whiteColor,50,SCREEN_WIDTH/2-100,SCREEN_HEIGHT/2 - 100,6);
        sysVideoRefresh();

        char end;
        while ((end = getchar())!= ' ');
        clearScreen(BACKBUFFER);
        sysVideoRefresh();
    }
}

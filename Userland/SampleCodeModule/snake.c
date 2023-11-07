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
Player  player1;
Player  player2;

void start_snake();
static void gameOverScreen();
static void menuScreen();
static void drawFrame(int mode,Player player);
static AxisPoint getRandomPoint();
static void setup(int mode);
static void readControls();
static void drawSnake(Player player);

static void moveUp(Player player);
static void moveDown(Player player);
static void moveLeft(Player player);
static void moveRight(Player player);
static void eatApple(Player player);
void checkCollision(int movDir);
static void borders();
static void readControls2();

int lastmoveDir1;
int lastmoveDir2;

static void setPlayerPosition(uint64_t startX,uint64_t startY,Player player)
{
    player.playerPos[0].x = startX;
    player.playerPos[0].y = startY;

    for (int i=1;i<player.currentSize;i++)
    {
        player.playerPos[i].x = startX+DOT_SIZE*i;
        player.playerPos[i].y = startY;
    }
}

static void setup(int mode)
{
    //Player set up
    player1.points = 0;
    player1.isAlive = 1;
    player1.color = whiteColor;
    player1.moveDir = 3; // <--
    player1.currentSize = PLAYER_INIT_SIZE;

    if(mode==2){
    player2.points = 0;
    player2.isAlive = 1;
    player2.color = whiteColor;
    player2.moveDir = 3; // <--
    player2.currentSize = PLAYER_INIT_SIZE;
    setPlayerPosition(SCREEN_WIDTH*0.25,SCREEN_HEIGHT/2,player1);
    setPlayerPosition(SCREEN_WIDTH*0.75,SCREEN_HEIGHT/2,player2);
    }

    
    if(mode==1){
    setPlayerPosition(SCREEN_WIDTH/2,SCREEN_HEIGHT/2,player1);}


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
    lastmoveDir1 = player1.moveDir;
    switch (c)
    {
        case 'w':
            if (player1.moveDir != 2)
            player1.moveDir = 1;
            break;
        case 's':
            if (player1.moveDir != 1)
                player1.moveDir = 2;
            break;
        case 'd':
            if (player1.moveDir != 3)
                player1.moveDir = 4;
            break;
        case 'a':
            if (player1.moveDir != 4)
                player1.moveDir = 3;
            break;
        case ESC:
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player1.isAlive =0;
            sysVideoRefresh();
            return;
    }
}

static void readControls2()
{
    char c;
    sysReadFromBuffer(&c);
    lastmoveDir2 = player2.moveDir;
    switch (c)
    {
        case 'i':
            if (player2.moveDir != 2)
            player2.moveDir = 1;
            break;
        case 'k':
            if (player2.moveDir != 1)
                player2.moveDir = 2;
            break;
        case 'l':
            if (player2.moveDir != 3)
                player2.moveDir = 4;
            break;
        case 'j':
            if (player2.moveDir != 4)
                player2.moveDir = 3;
            break;
        case ESC:
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player2.isAlive =0;
            sysVideoRefresh();
            return;
    }
}


static void playerMovement(Player player,int mode)
{
    int moveDir = player.moveDir;
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
            moveUp(player);
        break;
    case 2:
        if (lastmoveDir != 1)
            moveDown(player);
        break;
    case 3:
        if (lastmoveDir != 4)
            moveLeft(player);
        break;
    case 4:
        if (lastmoveDir != 3)
            moveRight(player);
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
static void drawFrame(int mode,Player player)
{
    if(mode==1){
    sysDrawCustomNumber(player1.points,grayColor, SCREEN_WIDTH/2,50, 6);}
    if(mode==2){
        sysDrawCustomNumber(player1.points,grayColor, SCREEN_WIDTH*0.25,50, 6);
    }
    if(mode==3){
        sysDrawCustomNumber(player2.points,grayColor, SCREEN_WIDTH*0.75,50, 6);
    }
    sysDrawFilledRect(redColor,apple.applePos.x, apple.applePos.y, apple.size, apple.size);

    drawSnake(player);
    borders();
    sysVideoRefresh(); //se imprime en pantalla
}


static void drawSnake(Player player)
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
static void updatePlayerTail(AxisPoint lastPosition,Player player)
{
    player.playerPos[1] = lastPosition;
    for (int i=player.currentSize-1;i>1;i--)
    {
        player.playerPos[i] = player.playerPos[i-1];
    }

}

static void moveUp(Player player)
{
    if (validate(player.playerPos[0].x,player.playerPos[0].y-DOT_SIZE) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].y=player.playerPos[0].y - DOT_SIZE;
        updatePlayerTail(lastPostion,player);

    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 
}
static void moveDown(Player player)
{
    if (validate(player.playerPos[0].x,player.playerPos[0].y+DOT_SIZE) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].y=player.playerPos[0].y + DOT_SIZE;
        updatePlayerTail(lastPostion,player);
    }
    else
    {
            sysPlayBeep();
            clearScreen(BACKBUFFER);
            player.isAlive =0;
            sysVideoRefresh();
    } 
}
static void moveLeft(Player player)
{
    if (validate(player.playerPos[0].x - DOT_SIZE,player.playerPos[0].y) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].x=player.playerPos[0].x - DOT_SIZE;
        updatePlayerTail(lastPostion,player);
    }
    else    
    {
        sysPlayBeep();
        clearScreen(BACKBUFFER);
        player.isAlive =0;
        sysVideoRefresh();
    } 

}
static void moveRight(Player player)
{
    if (validate(player.playerPos[0].x + DOT_SIZE,player.playerPos[0].y) != whiteColor)
    {
        AxisPoint lastPostion = player.playerPos[0];
        player.playerPos[0].x=player.playerPos[0].x  + DOT_SIZE;
        updatePlayerTail(lastPostion,player);
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

static void eatApple(Player player)
{
    player.currentSize++;
    apple.applePos = getRandomPoint();
    //Increse Size
    player.playerPos[player.currentSize].x = player.playerPos[player.currentSize-1].x + DOT_SIZE;
    player.playerPos[player.currentSize].y = player.playerPos[player.currentSize-1].y;

}


static void increase_score(Player player)
{
    if(validate(player.playerPos[0].x,player.playerPos[0].y) == redColor){
        eatApple(player);
        player.points++;
        drawSnake(player);
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
    if (c == '1')
    {
         setup(1);
        while (player1.isAlive != 0)
        {
            drawFrame(1,player1);
            clearScreen(BACKBUFFER);
            readControls();
            playerMovement(player1,1);
            increase_score(player1);
        }
    }
    if (c == '2')
    {
        setup(2);
          while (player1.isAlive != 0 && player2.isAlive!=0 )
        {
            drawFrame(2,player1);
            clearScreen(BACKBUFFER);
            readControls();
            playerMovement(player1,1);
            increase_score(player1);

             drawFrame(3,player2);
            clearScreen(BACKBUFFER);
            readControls2();
            playerMovement(player2,2);
            increase_score(player2);
        }
        
    }

    
    if(player1.isAlive == 0|| player2.isAlive == 0){
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

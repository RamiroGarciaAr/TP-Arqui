#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <color.h>
#include <point.h>

#define MAX_LENGTH 256

typedef struct player{
    int points; //amount of apples eaten
    int isAlive; 

    AxisPoint playerPos[MAX_LENGTH] ;
    
    int moveDir; // 1=up , 2=down, 3=left, 4=right
    int currentSize; //current Snake size
    uint32_t color;
}Player;

#endif
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <color.h>

typedef struct player
{
    int points; //amount of apples eaten
    bool isAlive; 
    
    uint64_t xAxis; // pos X
    uint64_t yAxis; // pos Y

    int moveDir; // 1=up , 2=down, 3=left, 4=right
    int initialSize; //start of the starting snake

    Color color;


}

#endif
//#include <syscalls.h>
#include <color.h>
#include <string.h>
#include <stdio.h>

/*
Macros by gcc library that makes variadic functions possible
*/
#include <stdarg.h>

#define BUFFER_MAX_LENGTH 250
#define TMP_LENGTH 150

/*
printf implementation. Fuente: 
https://www.equestionanswers.com/c/c-printf-scanf-working-principle.php#:~:text=printf%20or%20print%20function%20in,arguments%20function%20or%20vararg%20function.
Accepts integers, pointers, chars, strings, and hex numbers, not supported with floating point numbers.
*/
void printf(const char *str, ...){
    char *lengthErr = "Exceded length of the buffer.\n";
    int length = stringLength(str);
    if(length > BUFFER_MAX_LENGTH){
        _write(STDERR, lengthErr, stringLength(lengthErr));
        return;
    }    
    va_list vl;
    char buff[BUFFER_MAX_LENGTH] = {0};
    char tmp[TMP_LENGTH] = {0};
    int i=0, j=0;
    va_start(vl, str);
    while((str[i]) && j < BUFFER_MAX_LENGTH){
        if(str[i] == '%'){
            i++;
            switch(str[i]){
                case 'c':{
                    buff[j++] = (char) va_arg( vl, int );
                    break;
                }
                case 'd':{
                    char *auxStr = buff + j;
                    numToString((int) va_arg(vl, int), tmp, TMP_LENGTH);
                    stringCopy(auxStr, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);                    
                    break;
                }
                case 'x':{
                    char *auxStr = buff + j;
                    uintToBase((int) va_arg(vl, int), tmp, 16);
                    stringCopy(auxStr, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);
                    break;
                }
                case 's':{
                    char *auxStr = (char*) va_arg(vl, char *);
                    char *auxStr2 = buff + j;
                    stringCopy(tmp, TMP_LENGTH, auxStr);
                    stringCopy(auxStr2, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);
                    break;
                }
                case 'p':{
                    char *auxStr = buff + j;
                    int *pointer = (int *) va_arg(vl, int *);
                    decToHex((long) pointer, tmp, TMP_LENGTH);
                    stringCopy(auxStr, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);
                    break;
                }
                default:{
                    char *argErr = "Expected 's', 'd', 'x', 'p' or 'c' after '%'.\n";
                    _write(STDERR, argErr, stringLength(argErr));
                    return;
                }
            } 
        } else
            buff[j++] = str[i]; 
        i++;
    }
    
    if(j > BUFFER_MAX_LENGTH){
        char *buffOverflow = "Reached a buffer Overflow.\n";
        _write(STDERR, buffOverflow, stringLength(buffOverflow));
        return;
    }

    _write(STDOUT, buff, j);
    for(int i=0; i<BUFFER_MAX_LENGTH ;buff[i++] = 0);
    for(int i=0; i<TMP_LENGTH ; tmp[i++] = 0);
}

void scanf(const char* str, ...){
    va_list vl;
    char buff[BUFFER_MAX_LENGTH], tmp[TMP_LENGTH], c;
    int i=0, j=0, ret=0;
    va_start(vl, str);
    while((c = getChar()) != '\n' && c > 0 && i < BUFFER_MAX_LENGTH){
        buff[i++] = c;
    }
    va_start(vl, str);
    i = 0;
    while(str && str[i]){
        if(str[i] == '%'){
            if(buff[j] == '\0'){
                char *notEnough = "Not enough content entered.\n";
                _write(STDERR, notEnough, stringLength(notEnough));
                return;
            }
            i++;
            switch(str[i]){
                case 'c':{
                    *(char *) va_arg(vl, char*) = buff[j++];
                    ret++;
                    break;
                }
                case 'd':{
                    if(!isCharNum(buff[j])){
                        char * numErr = "Expected number.\n";
                        _write(STDERR, numErr, stringLength(numErr));
                        return;
                    }
                    int aux = 0;
                    while(aux < 50 && isCharNum(buff[j]) && buff[j] != '\0'){
                        tmp[aux++] = buff[j++];
                    }
                    tmp[aux] = '\0';
                    *(int *) va_arg(vl, int*) = stringToNum(tmp);
                    ret++;
                    break;
                }
                case 'x':{
                    if(!isCharNum(buff[j])){
                        char * numErr = "Number expected.\n";
                        _write(STDERR, numErr, stringLength(numErr));
                        return;
                    }
                    int aux = 0;
                    while(aux < 50 && isCharNum(buff[j]) && buff[j] != '\0'){
                        tmp[aux++] = buff[j++];
                    }
                    tmp[aux] = '\0';
                    *(int *) va_arg(vl, int*) = stringToNum(tmp);
                    ret++;
                    break;
                }
                default:{
                    if(!isCharNum(str[i])){
                        char *argErr = "Expected 'xxs', 'd', 'x', 'p' or 'c' after '%'.\n";
                        _write(STDERR, argErr, stringLength(argErr));
                        return;
                    }
                    int dimString = 0;
                    while(isCharNum(str[i])){
                        dimString *= 10;
                        dimString += str[i] - '0';
                    }
                    if(str[i] != 's'){
                        char *argErr = "Expected 'xxs', 'd', 'x', 'p' or 'c' after '%'.\n";
                        _write(STDERR, argErr, stringLength(argErr));
                        return;
                    }
                    char * toWrite = va_arg(vl, char*);
                    stringCopy(toWrite, dimString, &buff[j]);
                    j += stringLength(toWrite) - 1;
                    break;
                }
            }
        } else{
            char *argErr = "Expected a string with only %XX parameters as first argument";
            _write(STDERR, argErr, stringLength(argErr));
            return;
        }
        i++;
    }
}

void stringFormat(char *dest, unsigned int destSize, const char *str, ...){
    char *lengthErr = "Exceded length of the buffer";
    int length = stringLength(str);
    if(length > BUFFER_MAX_LENGTH || destSize > BUFFER_MAX_LENGTH){
        _write(STDERR, lengthErr, stringLength(lengthErr));
        return;
    }    
    va_list vl;
    char buff[BUFFER_MAX_LENGTH] = {0};
    char tmp[TMP_LENGTH] = {0};
    int i=0, j=0;
    va_start(vl, str);
    while((str[i]) && j < BUFFER_MAX_LENGTH){
        if(str[i] == '%'){
            i++;
            switch(str[i]){
                case 'c':{
                    buff[j++] = (char) va_arg( vl, int );
                    break;
                }
                case 'd':{
                    char *auxStr = buff + j;
                    numToString((int) va_arg(vl, int), tmp, TMP_LENGTH);
                    stringCopy(auxStr, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);                    
                    break;
                }
                case 'x':{
                    char *auxStr = buff + j;
                    uintToBase((int) va_arg(vl, int), tmp, 16);
                    stringCopy(auxStr, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);
                    break;
                }
                case 's':{
                    char *auxStr = (char*) va_arg(vl, char *);
                    char *auxStr2 = buff + j;
                    stringCopy(tmp, TMP_LENGTH, auxStr);
                    stringCopy(auxStr2, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);
                    break;
                }
                case 'p':{
                    char *auxStr = buff + j;
                    int *pointer = (int *) va_arg(vl, int *);
                    decToHex((long) pointer, tmp, TMP_LENGTH);
                    stringCopy(auxStr, BUFFER_MAX_LENGTH - j, tmp);
                    j += stringLength(tmp);
                    break;
                }
                default:{
                    char *argErr = "Expected 's', 'd', 'x', 'p' or 'c' after '%' ";
                    _write(STDERR, argErr, stringLength(argErr));
                    return;
                }
            } 
        } else
            buff[j++] = str[i]; 
        i++;
    }
    
    if(j > BUFFER_MAX_LENGTH){
        char *buffOverflow = "Reached a buffer Overflow";
        _write(STDERR, buffOverflow, stringLength(buffOverflow));
        return;
    }
    stringCopy(dest, destSize, buff);
    for(int i=0; i<BUFFER_MAX_LENGTH ;buff[i++] = 0);
    for(int i=0; i<TMP_LENGTH ;tmp[i++] = 0);
}


void printString(char* buffer) {
    _write(STDOUT, buffer, stringLength(buffer));
}

char getChar() {
    char c = 0;
    int count = _read(STDIN, &c, 1);
    if(count < 0)
        return -1;
    return c;
}

int putChar(char c){
    return _write(STDOUT, &c, 1);
}
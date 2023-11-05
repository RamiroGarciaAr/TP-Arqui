#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#define STDIN 0
#define STDOUT 1
#define BACKBUFFER 0
#define FRAMEBUFFER 1

//constantes de shell
#define EXIT_KEY 27
#define REGS_KEY 29
#define MAXSIZE 15
#define CANTREGS 18
#define CHARHEIGHT 16
#define CHARWIDTH 8
#define MAXINPUTLEN 50


//funciones de la libreria estandar y otras mas especificas para las operaciones requeridas
//si bien no todas se usan
int pow(int base, unsigned int exp);
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);
void getTime(char * timeBuff);

void clearScreen(int buffer);
int getScreenWidth();
int getScreenHeight();
void setCursorPos(int x, int y);

char getchar();
char * scanf();

void putchar(char c);
void print(char* string);
void printf(char* string, uint32_t color);
void println(char* string);
void printint(uint64_t value);
void zoomIn();
void zoomOut();

int strtok(char * s, char delim, char * array[], int arraySize);
int strlen(char * s);
int strcmp(char * s1, char * s2);
int getRandomInBetween(int max,int min);
int getRandom();
int getSize();

#endif //UTILS_H

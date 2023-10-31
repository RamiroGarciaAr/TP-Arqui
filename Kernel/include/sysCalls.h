#ifndef _SYSCALLS_H
#define _SYSCALLS_H
#include <stdint.h>
#include <videoDriver.h>
#include <keyboardDriver.h>

//constantes utilizadas en la system calls
//file descriptors
#define STDIN 0
#define STDOUT 1
#define KBDIN 3

#define READ 0
#define WRITE 1
#define DRAW 2
//color en hexa para imprimir en la shell
#define DEFAULTCOLOR 0xFFFFFF
#define ZOOM_RATE 5


uint64_t write(uint64_t fd, const char * buf, uint64_t len, uint64_t empty1, uint64_t empty2); // me retorna len
uint64_t customWrite(uint64_t fd, const char* buf, uint32_t color, uint64_t len, uint64_t empty1); // me retorna la posicion actual de la pen
uint64_t getScrWidth(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t getScrHeight(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t getCurrentTime(uint64_t mode, uint64_t timezone, uint64_t empty1, uint64_t empty2, uint64_t empty3);                                                                                        // bits 0-15 X ; bits 16-31 Y
uint64_t setCursorPosition(uint64_t x, uint64_t y, uint64_t empty1, uint64_t empty2, uint64_t empty3);
uint64_t read(uint64_t fd, char* buf, uint64_t len, uint64_t empty1, uint64_t empty2);
uint64_t drawFilledRect(uint32_t color, uint64_t x, uint64_t y, uint64_t width, uint64_t height);
uint64_t dottedLine(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t videoRefresh(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t clearScreen(uint64_t mode, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t checkKeyboardActivity(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t drawVideoChar(char number, uint32_t color, uint64_t x, uint64_t y, uint64_t size);
uint64_t readCharFromBuffer(char* buf, uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4);
uint64_t getPtrToPixel(uint64_t x, uint64_t y, uint32_t* color, uint64_t empty4, uint64_t empty5);
uint64_t zoomIn(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);
uint64_t zoomOut(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5);

// rax es el indice de acceso al vector estatico de todos los punteros a funcion de todos los handlers de las distintas sysCalls
uint64_t syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t rax);
void saveRegisters();


#endif
#ifndef SCREEN_DRIVER_H
#define SCREEN_DRIVER_H
#include <stdint.h>
#include <videoDriver.h>

#define BPP 3
#define TABLENGTH 5

//funcion para borrar el contenido del buffer de video ya sea frameBuffer o backBuffer segun el color de fondo definido
void clearBuffer(int buffer);

//funciones para obtener la posición del cursor que se usa para imprimir en la shell
uint16_t getCurrentY();

uint16_t getCurrentX();

//función para cambiar la posición del cursor
void setDrawingPosition(uint16_t x, uint16_t y);

//funciones para imprimir cadenas de caracteres usando el color definido hasta el momento
void print(const char * s);

void println(const char * s);

//funcion para manejar el tab, enter y backspace como caracteres especiales dado que no se guardaran en el buffer
//se crea un array de punteros a funcion (void*) llamando a la funcion especifica para el caracter
void manageSpecialCharacter(char c);

void newLine();

void tab();

void backspace();

#endif
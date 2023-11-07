#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H
#include <stdint.h>
#include <color.h>

//a diferencia de video driver no se manejan las posiciones del cursor para dibujar
//solo imprime en funcion de lo pedido y contiene los buffers
#define BUFFER_HEIGHT 768
#define BUFFER_WIDTH 1024
#define BACKBUFFER 0
#define FRAMEBUFFER 1
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16

//funciones para dibujar un pixel de color usando el frameBuffer (Front) y el backBuffer (Back)
void drawPixelFront(Color color, uint32_t x, uint32_t y);
void drawPixelBack(Color color, uint32_t x, uint32_t y);

//getters para el tamaño de la pantalla en pixeles
uint16_t getScreenHeight();

uint16_t getScreenWidth();

// funcion que devuelve el color seteado para dibujar
Color getDrawingColor();

// funcion para definir el color de los caracteres a dibujar
void setDrawingColor(Color color);

// funcion para imprimir un caracter en la posicion actual de drawingPosition segun el color pasado por parametro
void drawConsoleCharacter(char c, Color color);

// fuciones para obtener los buffer usados
uint8_t * getFrameBuffer();

uint8_t * getBackBuffer();

//funciones para obtener y modificar el color de fondo de la pantalla
Color getBackgroundColor();

uint8_t getBitsPerPixel();

void setBackgroundColor(Color color);

// funcion para pasar el contenido del backBuffer al frameBuffer para que sea visible para el usuario
void refreshVideo();

//funcion utilizada para dibujar figuras rectangulares
void drawFilledRectangle(Color color, uint32_t x, uint32_t y, uint32_t width, uint32_t height);

//funcion utilizada para imprimir un caracter en el backBuffer con un color, tamaño (entero) y posicion definidos
void drawCustomCharBack(char c, Color color, uint64_t x, uint64_t y);
void changeSize(uint64_t newSize);

#endif //VIDEODRIVER_H

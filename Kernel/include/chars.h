#ifndef _CHARS_H
#define _CHARS_H

//constantes que dfinen el rango de caracteres ascii con representacion grafica
#define LOWEST_CHAR 32
#define HIGHEST_CHAR 126

/* mapa de bits que indica con un 1 si el pixel esta prendido y 0 si esta apagado
 * de forma tal de imprimir un caracter de 16 x 8
 * Se accede a cada entrada por el ascii del caracter
 * Se utilizo la siguiente tabla adaptandola al tamaño que elegimos:
 * https://courses.cs.washington.edu/courses/cse457/98a/tech/OpenGL/font.c
 */
const char bitRepresentation[HIGHEST_CHAR-LOWEST_CHAR+1][16] = {
        {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
        {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x66,0x66,0x66,0x24,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x36,0x36,0x7F,0x36,0x36,0x36,0x7F,0x36,0x36,0x00,0x00,0x00},
        {0x00,0x18,0x18,0x3E,0x63,0x43,0x03,0x3E,0x60,0x60,0x61,0x63,0x3E,0x18,0x18,0x00},
        {0x00,0x00,0x00,0x00,0x43,0x63,0x30,0x18,0x0C,0x06,0x63,0x61,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x1C,0x36,0x36,0x1C,0x6E,0x3B,0x33,0x33,0x33,0x6E,0x00,0x00,0x00},
        {0x00,0x00,0x0C,0x0C,0x0C,0x0C,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x30,0x18,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x18,0x30,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x0C,0x18,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x0C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x10,0x10,0x0C,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x40,0x60,0x30,0x18,0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x66,0xC3,0xC3,0xDB,0xDB,0xC3,0xC3,0x66,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x18,0x1C,0x1E,0x18,0x18,0x18,0x18,0x18,0x18,0x7E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x60,0x30,0x18,0x0C,0x06,0x03,0x63,0x7F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x60,0x60,0x3C,0x60,0x60,0x60,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x30,0x38,0x3C,0x36,0x33,0x7F,0x30,0x30,0x30,0x78,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x7F,0x03,0x03,0x03,0x3F,0x60,0x60,0x60,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x1C,0x06,0x03,0x03,0x3F,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x7F,0x63,0x60,0x60,0x30,0x18,0x0C,0x06,0x03,0x03,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x42,0x42,0x42,0x3C,0x42,0x42,0x42,0x42,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x7E,0x60,0x60,0x60,0x30,0x1E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00,0x18,0x10,0x10,0x0C,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x60,0x30,0x18,0x0C,0x06,0x0C,0x18,0x30,0x60,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x06,0x0C,0x18,0x30,0x60,0x30,0x18,0x0C,0x06,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x63,0x30,0x18,0x18,0x18,0x00,0x18,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x3E,0x63,0x63,0x7B,0x7B,0x7B,0x3B,0x03,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x08,0x1C,0x36,0x63,0x63,0x7F,0x63,0x63,0x63,0x63,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3F,0x66,0x66,0x66,0x3E,0x66,0x66,0x66,0x66,0x3F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x66,0x43,0x03,0x03,0x03,0x03,0x43,0x66,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x1F,0x36,0x66,0x66,0x66,0x66,0x66,0x66,0x36,0x1F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x7F,0x66,0x46,0x16,0x1E,0x16,0x06,0x46,0x66,0x7F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x7F,0x66,0x46,0x16,0x1E,0x16,0x06,0x06,0x06,0x0F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x66,0x43,0x03,0x03,0x7B,0x63,0x63,0x66,0x5C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x7F,0x63,0x63,0x63,0x63,0x63,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x78,0x30,0x30,0x30,0x30,0x30,0x33,0x33,0x33,0x1E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x67,0x66,0x66,0x36,0x1E,0x1E,0x36,0x66,0x66,0x67,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x0F,0x06,0x06,0x06,0x06,0x06,0x06,0x46,0x66,0x7F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xC3,0xE7,0xFF,0xFF,0xDB,0xC3,0xC3,0xC3,0xC3,0xC3,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x63,0x67,0x6F,0x7F,0x7B,0x73,0x63,0x63,0x63,0x63,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3F,0x66,0x66,0x66,0x3E,0x06,0x06,0x06,0x06,0x0F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x63,0x6B,0x7B,0x3E,0x30,0x70,0x00},
        {0x00,0x00,0x00,0x3F,0x66,0x66,0x66,0x3E,0x36,0x66,0x66,0x66,0x67,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3E,0x63,0x63,0x06,0x1C,0x30,0x60,0x63,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xFF,0xDB,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0xC3,0x66,0x3C,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xC3,0xC3,0xC3,0xC3,0xC3,0xDB,0xDB,0xFF,0x66,0x66,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xC3,0xC3,0x66,0x3C,0x18,0x18,0x3C,0x66,0xC3,0xC3,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xC3,0xC3,0xC3,0x66,0x3C,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0xFF,0xC3,0x61,0x30,0x18,0x0C,0x06,0x83,0xC3,0xFF,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x0C,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0E,0x1C,0x38,0x70,0x60,0x40,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x3C,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x3C,0x00,0x00,0x00},
        {0x00,0x08,0x1C,0x36,0x63,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0x00},
        {0x00,0x0C,0x0C,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x1E,0x30,0x3E,0x33,0x33,0x33,0x6E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x07,0x06,0x06,0x1E,0x36,0x66,0x66,0x66,0x66,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x03,0x03,0x03,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x38,0x30,0x30,0x3C,0x36,0x33,0x33,0x33,0x33,0x6E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x7F,0x03,0x03,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x1C,0x36,0x26,0x06,0x0F,0x06,0x06,0x06,0x06,0x0F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x33,0x3E,0x30,0x33,0x1E},
        {0x00,0x00,0x00,0x07,0x06,0x06,0x36,0x6E,0x66,0x66,0x66,0x66,0x67,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x18,0x18,0x00,0x1C,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x60,0x60,0x00,0x70,0x60,0x60,0x60,0x60,0x60,0x60,0x66,0x66,0x3C},
        {0x00,0x00,0x00,0x07,0x06,0x06,0x66,0x36,0x1E,0x1E,0x36,0x66,0x67,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x1C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x3C,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x67,0xFF,0xDB,0xDB,0xDB,0xDB,0xDB,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x66,0x66,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x63,0x63,0x63,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x66,0x66,0x66,0x66,0x66,0x3E,0x06,0x06,0x0F},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x6E,0x33,0x33,0x33,0x33,0x33,0x3E,0x30,0x30,0x78},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3B,0x6E,0x66,0x06,0x06,0x06,0x0F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x63,0x06,0x1C,0x30,0x63,0x3E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x08,0x0C,0x0C,0x3F,0x0C,0x0C,0x0C,0x0C,0x6C,0x38,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x33,0x33,0x33,0x33,0x33,0x33,0x6E,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0xC3,0xC3,0xC3,0xC3,0x66,0x3C,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0xC3,0xC3,0xC3,0xDB,0xDB,0xFF,0x66,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0xC3,0x66,0x3C,0x18,0x3C,0x66,0xC3,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x63,0x63,0x63,0x63,0x63,0x63,0x7E,0x60,0x30,0x1F},
        {0x00,0x00,0x00,0x00,0x00,0x00,0x7F,0x33,0x18,0x0C,0x06,0x63,0x7F,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x70,0x18,0x18,0x18,0x0E,0x18,0x18,0x18,0x18,0x70,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x18,0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x18,0x18,0x00,0x00,0x00},
        {0x00,0x00,0x00,0x0E,0x18,0x18,0x18,0x70,0x18,0x18,0x18,0x18,0x0E,0x00,0x00,0x00}
};

#endif //_CHARS_H
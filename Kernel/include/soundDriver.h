#ifndef SOUND_DRIVER_H_
#define SOUND_DRIVER_H_
#include <stdint.h>
#include <interrupts.h>

//funcion para emitir un beep
void beep();
//funciones en asm que se comunican con los puertos para indicar las frecuencias a usar
extern void outb_asm();
extern uint8_t inb_asm(uint16_t port);

#endif
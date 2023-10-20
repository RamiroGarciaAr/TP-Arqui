#ifndef CLOCK_H
#define CLOCK_H

//constantes definidas para los modos que recibe la función clock
#define SECONDS 0X00
#define MINUTES 0X02
#define HOURS 0x04

//constante definida para adaptar el horario de UTC a nuestro uso horario de forma manual
#define TIME_ZONE -3 // sino se puede configurar por los flags de qemu

/*
 * funcion en asm que escribe en el puerto 70h el dato del realTimeClock que se necesita segun las constantes
 * y luego lee del puerto 71h el dato pedido de la hora actual segun UTC con formato bcd
 *
 */
extern unsigned char clock(unsigned char mode);

//funciones que llaman a clock para obtener el dato pedido y luego tra
unsigned int seconds();
unsigned int minutes();
unsigned int hours();


#endif

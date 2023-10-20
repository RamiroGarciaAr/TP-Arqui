#include <clock.h>
#include <stdint.h>

/* funcion static que convierte el valor en formato bcd obtenido de clock a decimal
 * Se obtiene la primer cifra usando un decalaje sobre la parte alta del byte
 * asegurandose que a la izquierda van a quedar 0's
 * como define las decenas se lo multiplica por 10
 * y se le suman las unidades cuyo digito esta en la parte baja del byte
 * para eso se usa una mascara con 0xOF y se suma directamente el valor obtenido
 */
static unsigned int decode(unsigned char time){
    return (time >> 4) * 10 + (time & 0x0F);
}

unsigned  int seconds(){
    return decode(clock(SECONDS));
}

unsigned int minutes(){
    return decode(clock(MINUTES));
}

//se acomoda el valor a la zona horaria
unsigned int hours(){
    return decode(clock(HOURS)) + TIME_ZONE;
}


#ifndef __KEYBOARDRIVER_H__
#define __KEYBOARDRIVER_H__

//constantes que se van a usar para evitar magic numbers
//valores obtenidos de: https://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/codes.h pasados a decimal
#define MAX_SCANCODE  58
#define LSHFT_PRESSED 42
#define LSHFT_RELEASED (LSHFT_PRESSED + 128)
#define RSHFT_PRESSED 54
#define RSHFT_RELEASED (RSHFT_PRESSED + 128)
#define CAPSLOCK_PRESSED 58
#define CAPSLOCK_RELEASED (CAPSLOCK_PRESSED + 128)
#define ESC 27
#define ALT 56


//funcion de asm para leer un scanCode del teclado accediendo al puerto 60h
extern unsigned char getScanCode();

//funcion en asm para saber si se oprimio alguna tecla del teclado mirando el bit menos significativo que se leyo del puerto 64h
extern int isKeyboardPressed();

//devuelve 1 si no hay elementos en el buffer, 0 si hay chars Ascii
int isBufferEmpty();

//se guarda en el buffer el ascii asociado a la tecla presionada si es que tiene 
int saveKeyInBuffer();

//se devuelve el primer char ingresado al buffer actualizandose la cola que está definida para el mismo
char consumeKeyFromBuffer();

//int consumeScanCodeFromBuffer(); ver si lo agregamos al final

//se hace una limpieza de buffer
void clearKeyboardBuffer();
void getAllKeys(char * c);

#endif

#include <stdint.h>
#include <videoDriver.h>
#include <screenDriver.h>
#include <exceptions.h>
#include <keyboardDriver.h>
#include <interrupts.h>


// conversor de uint64_t a representacion hexa utilizado para imprimir el contenido de los registros
// ej: uint64ToHex(60,buf[16]) --> me deja buf = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,C}
static void uint64ToHex(uint64_t n, char buf[16]) {
    int digit;
    for (int i = 15; i >= 0; i--) {
        digit = n % 16;
        if (digit < 10) {
            buf[i] = '0' + digit;
        } else {
            buf[i] = 'A' + (digit - 10);
        }
        n /= 16;
    }
}


void exceptionsHandler(uint64_t code, const uint64_t registerValues[17]) {
    // creo un buffer para la impresion de los valores de los registros
    char hexbuf[17];
    hexbuf[16]='\0';

    Color red = {0x00, 0x00, 0xFF};
    Color white = {0xFF, 0xFF, 0xFF};
    // impriremos el mensaje en rojo
    setDrawingColor(red);
    print(errorMessage);
    switch(code) {
        case 0x00: print(exc_0_message); break;
        case 0x06: print(exc_6_message); break;
        default: print("Unkown type of exception."); break;
    }
    newLine();
    // imprimiremos el valor de los registros en blanco
    setDrawingColor(white);
    // primero los valores de RIP y RFLAGS
    for(int i=16; i<=17; i++) {
        print(registerNames[i]);
        print(": ");
        print("0x");
        uint64ToHex(registerValues[i], hexbuf);
        print(hexbuf);
        if(i==16) { print("  "); } else { print("\n"); }
    }
    // imrpimo el resto de los registros, 4 resgistros por linea
    for(int i = 0; i < 16; i++) {
        print(registerNames[i]);
        print(": ");
        print("0x");
        uint64ToHex(registerValues[i], hexbuf);
        print(hexbuf);
        if (i % 4 == 3)
            newLine();
        else
            print("   ");
    }

    // esperamos a que una tecla sea presionada nuevamente para retornar a la shell
    print("press any keys to continue");
    newLine();
    do {
        _hlt();
    } while (isBufferEmpty());
    // borramos lo que quedo en el buffer
    clearKeyboardBuffer();
    return;
}

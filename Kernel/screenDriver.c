#include <videoDriver.h>
#include <screenDriver.h>
#include <stdint.h>

uint16_t Xpos = 0, Ypos = 0;

uint16_t getCurrentY() {
    return Ypos;
}

uint16_t getCurrentX() {
    return Xpos;
}

static void setXPosition(uint16_t x) {
    Xpos = x ;
}

//analogo a setXposition
static void setYPosition(uint16_t y) {
    Ypos = y;
}

void setDrawingPosition(uint16_t x, uint16_t y) {
    setXPosition(x);
    setYPosition(y);
}
static uint32_t * copymem(uint8_t * from, uint8_t * to, uint64_t len) {
    for (uint64_t i = 0; i < len; i++) {
        to[i] = from[i];
    }
    return (uint32_t *)to;
}

void clearBuffer(int buffer) {
    uint8_t * pos = (buffer == FRAMEBUFFER)? getFrameBuffer() : getBackBuffer();
    uint32_t totalPixels =  (buffer == FRAMEBUFFER) ? (uint32_t)(getScreenWidth() * getScreenHeight()) : BUFFER_HEIGHT * BUFFER_WIDTH;
    uint32_t len = BPP*totalPixels;
    Color background = getBackgroundColor();
    //se recorre todos los pixeles de la pantalla poniendo un pixel con el color de fondo (negro) en cada uno de ellos
    while(len) {
        if (len % 3 == 0) {
            *pos = background.b;
        }
        else if (len % 3 == 1){
            *pos = background.g;
        }
        else{
            *pos = background.r;
        }
        pos++;
        len--;
    }
}

void newLine() {
    setXPosition(0);
    // si no hay espacio para avanzar la posicion en Y,
    // se mueve la pantalla entera hacia arriba y se borra la parte inferior
    //el 2 viene de que ademas de la linea actual tiene que entrar otra linea abajo (empiezo cada linea desde la posicion en y mas proxima a la ultima linea impresa)
    if(getScreenHeight() < (getCurrentY() + 2*CHAR_HEIGHT)) {
        //si entre es porque estoy en la ultima linea
        uint8_t * destination = getFrameBuffer();
        //el 3 viene de contar los bytes usados por cada pixel para definir el color
        uint8_t * from = (destination + BPP*(getScreenWidth()*CHAR_HEIGHT));
        //se calcula cuantos son los bytes que hay que copiar desde la segunda linea en adelante
        uint64_t len = BPP*((getScreenHeight()-CHAR_HEIGHT)*((uint64_t)getScreenWidth()));
        copymem(from,destination,len);
        //se calcula la cantidad de bytes a eliminar de una linea (para borrar la ultima)
        uint64_t clearLen = BPP*((uint64_t)getScreenWidth() * CHAR_HEIGHT);
        uint8_t *  clearDestination = destination + len;
        Color background = getBackgroundColor();
        //se borra la ultima completando con pixeles del color de fondo
        for(int i = clearLen - 1; i >= 0; i--) {
            if (i % 3 == 0) {
                clearDestination[i] = background.b;
            }
            else if (i % 3 == 1){
                clearDestination[i] = background.g;
            }
            else{
                clearDestination[i] = background.r;
            }
        }
    } else { //si queda espacio en la pantalla se pasa a la siguiente linea sin mover nada de lo ya impreso
        setYPosition(getCurrentY()+CHAR_HEIGHT);
    }
}

void tab(){
    //no se hace un chequeo del borde para poder modificar la posicion en y al hacer el print
    setXPosition(getCurrentX()+ TABLENGTH * CHAR_WIDTH);
}

void backspace(){
    if(getCurrentX() == 0){
        setDrawingPosition(getScreenWidth() - CHAR_WIDTH, getCurrentY()- CHAR_HEIGHT);
        drawConsoleCharacter(' ', getBackgroundColor());
        setDrawingPosition(getScreenWidth()-CHAR_WIDTH, getCurrentY()- CHAR_HEIGHT);
        return;
    }
    setDrawingPosition(getCurrentX()-CHAR_WIDTH,getCurrentY());
    drawConsoleCharacter(' ', getBackgroundColor());
    setDrawingPosition(getCurrentX()-CHAR_WIDTH,getCurrentY());
}


void manageSpecialCharacter(char c){
    //armamos un switch con los casos que tienen un comportamiento especial (no se hace un print de un char ascii)
    switch(c){
        case '\t':
            tab();
            break;
        case '\b':
            backspace();
            break;
        case '\n':
        case '\r':
            newLine();
            break;
    }
}

void print(const char * s) {
    while(*s) {
        drawConsoleCharacter(*s, getDrawingColor());
        s++;
    }
}

void println(const char * s) {
    print(s);
    newLine();
}
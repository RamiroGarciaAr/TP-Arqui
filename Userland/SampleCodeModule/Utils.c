#include <stdint.h>
#include <utils.h>
#include <syscallsManager.h>


//funcion provista por x64barebones originalmente en naiveconsole.c
uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base){
    char *p = buffer;
    char *p1, *p2;
    uint32_t digits = 0;

    //Calculate characters for each digit
    do
    {
        uint32_t remainder = value % base;
        *p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
        digits++;
    }
    while (value /= base);

    // Terminate string in buffer.
    *p = 0;

    //Reverse string in buffer.
    p1 = buffer;
    p2 = p - 1;
    while (p1 < p2)
    {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
    return digits;
}

void print(char* string){
    sysWrite(STDOUT, string, strlen(string));
}

//similar a scanf pero sin separador por \n->
char getchar(){
    char buf;
    sysRead(STDIN, &buf, 1);
    //getAscii()->
    return buf;
}

char * scanf(){
    char * string = 0;
    int i = 0;
    char c;
    while ((c = getchar()) != '\r'){
        string[i++] = c;
    }
    return string;
}

void printf(char* string, uint32_t color){
    sysCustomWrite(STDOUT, string,color, strlen(string));
}

void putchar(char c){
    print(&c);
}

void println(char* string){
    print(string);
    putchar('\n');
}

void printint(uint64_t value){
    char buffer[10];
    uintToBase(value,buffer,10);
    print(buffer);
}

void clearScreen(int buffer){
    //llamar a la syscall
    sysClearScreen((uint64_t)buffer);
}

void setCursorPos(int x, int y){
    sysSetCursorPos((uint64_t) x, (uint64_t) y);
}

int getScreenWidth(){
    return sysGetScrWidth();
}
int getScreenHeight(){
    return sysGetScrHeight();
}

int strcmp(char * s1, char * s2){
    //mientras ambos coincidan caracter a caracter continuo hasta el final
    char * second = s2;
    char * first = s1;
    while(*first != 0 && *second != 0 && *first == *second){
        first++;
        second++;
    }
    //necesito que ambos hayan llegado hasta el final
    return *first == 0 && *second == 0;
}

int strlen(char * s){
    int len = 0;

    while(*s != 0) {
        len++;
        s++;
    }
    return len;
}
//se devuelve la cant maxima de strings que se parsearon
int strtok(char * s, char delim, char * array[], int arraySize){
    int arrayIndex = 0;
    //primero se guarda el puntero en el array desde el principio
    //entonces por cada vez que encuentre el delimitador tengo que dejar el puntero al char de inicio
    if(*s != delim && *s != '\0')
        array[arrayIndex++] = s;
    while( *s != '\0' ){
        if(*s == delim){
            *s = 0;
            //si el siguiente no es tambien un delimitador ni el final del string
            //entonces mientras todavia tenga espacio en el array -> me guardo el puntero al siguiente
            if( *(s+1) != delim &&  (*(s+1) != '\0')){
                if(arrayIndex >= arraySize)
                    return arrayIndex;
                array[arrayIndex++] = s + 1;
            }
        }
        s++;
    }
    return arrayIndex;
}

int pow(int base, unsigned int exp){
    //si exp es 0 devuelvo 1
    int ans = 1;

    for (int i = 0; i < exp; i++)
        ans *= base;

    return ans;
}

int getRandomInBetween(int max,int min)
{
    if (max < min) return -1;
   return getRandom() % (max+1-min) + min;
}

int getRandom()
{
    uint64_t aux = sysGetTime();
    aux = ((aux*aux)/100)%1000;
    return aux;

}

void getTime(char * timeBuff) {
    char * p = timeBuff;
    uint64_t time = sysGetTime();
    uint64_t hours = time & 0xFF;
    if (hours < 10){
        p[0] = '0';
        uintToBase(hours,p+1, 10);
    }
    else {
        uintToBase(hours, p, 10); // cargo horas (bits 0-7 de time)
    }
    p[2] = ':';
    uint64_t minutes = (time >> 8) & 0xFF;
    if (hours < 10){
        p[3] = '0';
        uintToBase(hours,p+4, 10);
    }
    else {
        uintToBase(minutes, p + 3, 10); // cargo minutos (bits 8-15 de time)
    }
    p[5] = ':';
    uint64_t seconds = (time >> 16) & 0xFF;
    if (seconds < 10){
        p[6] = '0';
        uintToBase(hours,p+7, 10);
    }
    else {
        uintToBase(seconds, p + 6, 10); // cargo segundos (bits 16-23 de time)
    }
    p[8] = 0;
}

void zoomIn(){
    clearScreen(FRAMEBUFFER);
    sysZoomIn();
}

void zoomOut(){
    clearScreen(FRAMEBUFFER);
    sysZoomOut();
}

int getSize(){
   return sysGetSize();
}
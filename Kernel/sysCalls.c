#include <sysCalls.h>
#include <videoDriver.h>
#include <screenDriver.h>
#include <keyboardDriver.h>
#include <interrupts.h>
#include <clock.h>
#include <time.h>
#include <soundDriver.h>


static Color convertHexatoColor(uint64_t hexa){
    Color selectedColor;
    selectedColor.b = hexa & 255;
    selectedColor.g = (hexa>> 8) & 255;
    selectedColor.r = (hexa>> 16) & 255;
    return selectedColor;
}

uint64_t readCharFromBuffer(char* buf, uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4) {
    *buf = consumeKeyFromBuffer();
    return 0;
}

uint64_t read(uint64_t fd, char* buf, uint64_t len, uint64_t empty1, uint64_t empty2) {
    char reading;
    int i = 0;
    if (fd ==STDIN) {
        while(i < len) {
            saveKeyInBuffer();
            reading= consumeKeyFromBuffer();
            if (reading != 0){
                buf[i++] = reading;
            }
        }
        return (uint64_t) i;
    }
    return 0;
}

uint64_t customWrite(uint64_t fd, const char* buf, uint32_t color,uint64_t len, uint64_t empty1) {
    if (fd == STDOUT) {
        setDrawingColor(convertHexatoColor(color));
        print(buf);
        return getCurrentX() | ((uint32_t) getCurrentY() << 16);
    }
    return 0;
}

uint64_t write(uint64_t fd, const char * buf, uint64_t len, uint64_t empty1, uint64_t empty2) {
    return customWrite(fd, buf, DEFAULTCOLOR, len, empty2);
}

uint64_t setCursorPosition(uint64_t x, uint64_t y, uint64_t empty1, uint64_t empty2, uint64_t empty3){
    setDrawingPosition(x,y);
    return 0;
}

// RETORNO: bits 0-7 --> horas ; bits 8-15 --> minutos ; bits 16-23 --> segundos
uint64_t getCurrentTime(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5) {
    return (hours()) | (minutes() << 8) | (seconds() << 16);
}

uint64_t getScrWidth(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5){
    return getScreenWidth();
}
uint64_t getScrHeight(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5){
    return getScreenHeight();
}

uint64_t drawFilledRect(uint32_t color, uint64_t x, uint64_t y, uint64_t width, uint64_t height){
    drawFilledRectangle(convertHexatoColor(color), (uint32_t)x, (uint32_t) y, (uint32_t) width,(uint32_t) height);
    return 0;
}

uint64_t dottedLine(uint64_t color, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5){
    uint32_t rectangleWidth = 10;
    uint32_t rectangleHeight = 20;
    uint32_t centerX = getScreenWidth() / 2;
    uint32_t endY = getScreenHeight() - 1;

    for (uint32_t y=0; y <= endY; y += (rectangleHeight + (rectangleHeight / 2))) {
        drawFilledRectangle(convertHexatoColor(color), centerX - (rectangleWidth / 2), y, rectangleWidth, rectangleHeight);
    }
    return 0;
}

uint64_t videoRefresh(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5){
    refreshVideo();
    return 0;
}

uint64_t clearScreen(uint64_t mode, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5){
    clearBuffer(mode);
    return 0;
}

uint64_t playBeep(uint64_t empty1, uint64_t empty2, uint64_t empty3, uint64_t empty4, uint64_t empty5){
    beep();
    return 0;
}

uint64_t drawVideoChar(char character, uint32_t color, uint64_t x, uint64_t y){
    drawCustomCharBack(character, convertHexatoColor(color), x, y);
    return 0;
}
//se estandariza el prototipo de las funciones que atienden system calls a 5 parametros
static uint64_t (*syscalls[])(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8) =
        {read, readCharFromBuffer, write,customWrite, setCursorPosition,getCurrentTime, getScrWidth, getScrHeight, drawFilledRect, dottedLine, videoRefresh, clearScreen, playBeep, drawVideoChar};

uint64_t syscallHandler(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t r10, uint64_t r8, uint64_t r9) {
    if (r9 < (sizeof(syscalls)/sizeof(syscalls[0])) && syscalls[r9] != 0) {
        return syscalls[r9](rdi, rsi, rdx, r10, r8);
    }
    return 1;
}


#include <videoDriver.h>
#include <screenDriver.h>
#include <stdint.h>
#include <chars.h>

#include "include/font.h"

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;		// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;

	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;

	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;

// seteo el color en blanco por default
Color drawingColor = {0xFF,0xFF,0xFF};
Color backgroundColor = {0x00, 0x00, 0x00};

uint32_t cursorX =0;
uint32_t cursorY =0;

uint32_t fontSize = DEFAULT_FONT_SIZE;


void setFontSize(uint32_t newSize)
{
    fontSize = newSize;
}
uint32_t getFontSize()
{
    return fontSize;
}
//double buffer
uint8_t backBuffer[BUFFER_HEIGHT * BUFFER_WIDTH * 3] = {0x00};

uint8_t getBitsPerPixel(){
    return VBE_mode_info->bpp;
}

uint16_t getScreenHeight() {
    return VBE_mode_info->height;
}

uint16_t getScreenWidth() {
    return VBE_mode_info->width;
}

uint8_t * getFrameBuffer() {
    return (uint8_t *) ((uint64_t)VBE_mode_info->framebuffer);
}

uint8_t * getBackBuffer() {
    return backBuffer;
}

Color getDrawingColor() {
    return drawingColor;
}

Color getBackgroundColor() {
    return backgroundColor;
}

void setDrawingColor(Color color) {
    drawingColor = color;
}

void setBackgroundColor(Color color) {
    backgroundColor = color;
}

// funcion auxiliar para copiar n pixeles desde dos posiciones especificas de la pantalla
//es util para copiar lineas enteras al hacer scrolling
static uint32_t * copymem(uint8_t * from, uint8_t * to, uint64_t len) {
    for (uint64_t i = 0; i < len; i++) {
        to[i] = from[i];
    }
    return (uint32_t *)to;
}

void refreshVideo(){
    copymem(backBuffer, getFrameBuffer(), BUFFER_HEIGHT * BUFFER_WIDTH*3);
}


void drawPixelFront(Color color, uint32_t x, uint32_t y) {
    if(x >= VBE_mode_info->width || y >= VBE_mode_info->height) {
        return;
    }
    uint8_t * videoPtr = (uint8_t *) ((uint64_t)VBE_mode_info->framebuffer);
    int offset = y * VBE_mode_info->pitch + x * (VBE_mode_info->bpp / 8);
    videoPtr[offset] = color.b;
    videoPtr[offset+1] = color.g;
    videoPtr[offset+2] = color.r;
}

void drawPixelBack(Color color, uint32_t x, uint32_t y) {
    if(x >= VBE_mode_info->width || y >= VBE_mode_info->height) {
        return;
    }
    uint8_t * backBufferPtr = backBuffer;
    int offset = y * VBE_mode_info->pitch + x * (VBE_mode_info->bpp / 8);
    backBufferPtr[offset] = color.b;
    backBufferPtr[offset+1] = color.g;
    backBufferPtr[offset+2] = color.r;
}

static int isPrintableChar(char c) {
    return c >= LOWEST_CHAR && c <= HIGHEST_CHAR;
}

void drawConsoleCharacter(char c, Color color, uint64_t size) {
    if(isPrintableChar(c)) {
        Color printingColor = color;
        if (c == ' '){
            printingColor = backgroundColor;
        }
        int conversionRow = c - LOWEST_CHAR;
        for(int h=0; h<CHAR_HEIGHT*size; h++) {
            for(int i=0; i<CHAR_WIDTH*size; i++) {
                //si el bit no es un 0 se cambia
                if(bitRepresentation[conversionRow][h/size] & (1 << i/size)) {
                    drawPixelFront(printingColor,getCurrentX()+i, (getCurrentY())+h+size);
                }
            }
        }
    }
    else{
        manageSpecialCharacter(c);
        return;
    }
    //si no es un caracter printeable y tampoco requiere de una atencion especial
    // directamente se actualiza el puntero a la siguiente posicion valida a printear en pantalla
    setDrawingPosition(getCurrentX()+ CHAR_WIDTH*size, getCurrentY());
    //se chequea que no se haya llegado a un borde
    if(getCurrentX() > getScreenWidth() - CHAR_WIDTH*size) {
        newLine();
    }
}

void drawCustomCharBack(char c, Color color, uint64_t x, uint64_t y, uint64_t size) {
    int digit = c - LOWEST_CHAR;

    int startX = x;
    int startY = y;

    for (int h = 0; h < CHAR_HEIGHT; h++) {
        for (int w = 0; w < CHAR_WIDTH; w++) {
            if (bitRepresentation[digit][h] & (1 << w)) {
                for (int sh = 0; sh < size; sh++) {
                    for (int sw = 0; sw < size; sw++) {
                        drawPixelBack(color, startX + (w * size) + sw, startY + (h * size) + sh);
                    }
                }
            }
        }
    }
}

void drawCustomNumber(int num, Color color, uint64_t x, uint64_t y, uint64_t size){
    if(num>= 0 && num <= 9){
        drawCustomCharBack(num + '0', color, x,y,size);
    }
    else{
        drawCustomCharBack(num/10 + '0', color, x-50,y,size);
        drawCustomCharBack(num%10 + '0', color,x,y,size);
    }
}

void drawFilledRectangle(Color color, uint32_t x, uint32_t y, uint32_t width, uint32_t height){
    if(x>=getScreenWidth()||y>=getScreenHeight()) {
        return;
    }
    //a lo sumo llego hasta el borde
    if(x+width>getScreenWidth()) {
        width = getScreenWidth()-x;
    }
    // a lo sumo hasta el borde
    if(y+height>getScreenHeight()) {
        height = getScreenHeight()-y;
    }

    for(uint32_t i = 0; i < width; i++) {
        for(uint32_t j = 0; j < height; j++) {
            drawPixelBack(color,x+i,y+j);
        }
    }
}
void drawFilledSquare(uint32_t x,uint32_t y, uint32_t size, Color hexColor)
{
    drawFilledRectangle(hexColor,x,y,size,size);
}

Color* videoGetPtrToPixel(uint64_t x, uint64_t y) {
    /*
    Punto (x,y) en la pantalla. Como la memoria es continua, para llegar a ese punto es mediante la sig. ecuación:
    */
    Color* fbPtr = (Color*)((uint64_t)VBE_mode_info->framebuffer);

    return &fbPtr[VBE_mode_info->width * y + x];
}
//Screen Driver
/*
uint32_t getCursorX()
{
    return cursorX;
}
uint32_t getCursorY()
{
    return cursorY;
}
static void setXCursorPosition(uint32_t newXPos)
{
    cursorX = newXPos;
}
static void setYCursorPosition(uint32_t newYPos)
{
    cursorY = newYPos;
}
void setDrawingPosition(uint32_t xPos,uint32_t yPos)
{
    setYCursorPosition(yPos);
    setXCursorPosition(xPos);
}

/// New char system
uint64_t getPixelHex(uint32_t x, uint32_t y) {
	uint8_t * screen = (uint8_t *) ((uint64_t) (VBE_mode_info->framebuffer));
    uint32_t offset = VBE_mode_info->pitch*y + x*3;
    
    int b = screen[offset];
    int g = screen[offset+1];
    int r = screen[offset+2];
    return FROM_RGB(r,g,b);
}
void putPixel(uint64_t hexColor,uint32_t x,uint32_t y)
{
    uint8_t *screen = (uint8_t *) ((uint64_t)(VBE_mode_info->framebuffer));
    uint32_t offset = VBE_mode_info->pitch*y + x*3;   
    screen[offset] = TO_RED(hexColor);
    screen[offset+1] = TO_BLUE(hexColor);
    screen[offset+2] = TO_GREEN(hexColor);
}

void putSquare(uint32_t x,uint32_t y,uint32_t size,uint64_t hexColor)
{
    for (uint32_t i=0;i<size;i++)
    {
        for(uint32_t j=0; j<size;j++)
        {
            putPixel(hexColor,x+i,y+j);
        }
    }
}

void drawChar(uint64_t hexColor, char character)
{
    int a = cursorX;
    int x = a;
    int y = cursorY;
    int start = character - 33;

    if (character == ' ') return; // si es un espacio no dibuja nada
    if (isMinusc(character))
    {
        start = character - 'a';
    }
    for(int i =0; i<32;i++)
    {
        if (i%2==0 && i !=0)
        {
            y+=size; // Salto a la siguiente fila de píxeles
            a=x; // Reinicia la posición horizontal al inicio
        }
        font[i+(start * 32)] & (char)0x01 ? putSquare(a,y,size,hexColor):0;

        uint8_t aux = 0x02;

        for (int j =0;j<8;j++)
        {
             // Comprueba cada bit de la fuente y dibuja un píxel si está activo
            ((uint8_t)font[i + (start * 32)] & (uint8_t)aux) >> j ? putSquare(a, y, size, hexColor) : 0;
            a += size;  // Avanza a la siguiente posición horizontal
            aux <<= 1;  // Desplaza el bit auxiliar hacia la izquierda
        }

    }
}
void moveOneLineUp() {
    char* dst = (char*)(uintptr_t)(VBE_mode_info->framebuffer); // Puntero al framebuffer
    char* src = dst + VBE_mode_info->pitch * size * 16; // Puntero a la línea de origen
    uint64_t numBytes = VBE_mode_info->pitch * (VBE_mode_info->height - size * 16); // Cantidad de bytes a copiar

    memcpy(dst, src, numBytes); // Copia los bytes desde la línea de origen a la línea de destino
    //drawRectangle(bg_color, 0, VBE_mode_info->height - size*16, 1024, size*16 );
    //drawFilledRectangle(backgroundColor,0,VBE_mode_info->height - size*16,1024,size*16);
    cursorY -= (size * 16); // Actualiza la posición del cursor en el eje Y
}

void character(uint64_t hexColor, char c)
{
    //Special Chaaracters
    manageSpecialCharacter(c);
    //Screen Limits
    if (cursorX >= getScreenWidth())
    {
        cursorX=0;
        cursorY += size*16;
    }
    if (cursorY >= getScreenHeight())
    {
        cursorX =0;
        moveOneLineUp();
    }
    //Draw Character
    drawChar(hexColor,c);
    cursorX += size*8;
    return;
}
*/
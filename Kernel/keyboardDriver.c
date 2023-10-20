#include <keyboardDriver.h>
#include <screenDriver.h>

#define BUFFER_SIZE 15
//matriz de conversion para los scancodes con equivalencia en ascii
//la primer columna es el valor ascii obtenido por default
//la segunda columna es el valor ascii obtenido cuando está presionada la tecla shift o en las letras el capsLock
//se accede a la conversion por el scancode
//el mismo  coincide con el num de fila en la matriz
//obtenida de: https://www.cs.umd.edu/~hollings/cs412/s98/project/proj1/scancode
static char asccode[58][2] =
        {
                {   0,0   } ,
                { ESC,ESC } ,
                { '1','!' } ,
                { '2','@' } ,
                { '3','#' } ,
                { '4','$' } ,
                { '5','%' } ,
                { '6','^' } ,
                { '7','&' } ,
                { '8','*' } ,
                { '9','(' } ,
                { '0',')' } ,
                { '-','_' } ,
                { '=','+' } ,
                {   8,8   } ,
                {   9,9   } ,
                { 'q','Q' } ,
                { 'w','W' } ,
                { 'e','E' } ,
                { 'r','R' } ,
                { 't','T' } ,
                { 'y','Y' } ,
                { 'u','U' } ,
                { 'i','I' } ,
                { 'o','O' } ,
                { 'p','P' } ,
                { '[','{' } ,
                { ']','}' } ,
                {  13,13  } ,
                {  29,29  } ,
                { 'a','A' } ,
                { 's','S' } ,
                { 'd','D' } ,
                { 'f','F' } ,
                { 'g','G' } ,
                { 'h','H' } ,
                { 'j','J' } ,
                { 'k','K' } ,
                { 'l','L' } ,
                { ';',':' } ,
                {  39,34  } ,
                { '`','~' } ,
                {   0,0   } ,
                { '\\','|'} ,
                { 'z','Z' } ,
                { 'x','X' } ,
                { 'c','C' } ,
                { 'v','V' } ,
                { 'b','B' } ,
                { 'n','N' } ,
                { 'm','M' } ,
                { ',','<' } ,
                { '.','>' } ,
                { '/','?' } ,
                {   0,0   } ,
                {   0,0   } ,
                {   0,0   } ,
                { ' ',' ' } ,
        };

static int isLetter(int scancode);
static int isSpecialCharacter(int scancode);
static char convertToAscii(int scancode);
static int scanCodeHasAscii(int scancode);

//ponemos un flag para asegurarse si la key leida que no es una letra
//debe ser un letra mayuscula u otro character en la misma tecla
int shiftPressed = 0;
int capsLockPressed = 0;

//creo un buffer como un array de characters y vamos haciendo un resize de a chunks
char keyboardBuffer[BUFFER_SIZE];
unsigned int bufferSize = 0;

int saveKeyInBuffer(){
	int scancode;
    while(isKeyboardPressed()) {
        scancode = getScanCode();
        if (!isSpecialCharacter(scancode)) {
            if (scanCodeHasAscii(scancode)) {
                char character = convertToAscii(scancode);
                keyboardBuffer[bufferSize++] = character;
                return 1;
            }
        }
    }
	return 0;
}

int isBufferEmpty(){
    return bufferSize <= 0;
}

//trato al buffer como una cola
char consumeKeyFromBuffer(){
    if (isBufferEmpty() || keyboardBuffer[0] == 0){
        return 0;
    }
	char toReturn = keyboardBuffer[0];

    //muevo todo un lugar a la izquierda
	for (int i = 1; i < bufferSize; i++){
		keyboardBuffer[i-1] = keyboardBuffer[i];
	}
	//actualizo el size
	bufferSize--;
	return toReturn;
}

void clearKeyboardBuffer(){
    bufferSize = 0;
}

static char convertToAscii(int scancode){
   if(!scanCodeHasAscii(scancode)){
       	 return -1; //No tiene ascii
   }

   if(capsLockPressed && isLetter(scancode)){
        return asccode[scancode][capsLockPressed];
   }
   
   return asccode[scancode][shiftPressed > 0];
}

static int scanCodeHasAscii(int scancode){
	//si está en el rango de scan codes con equivalente en ASCII devuelve un 1
	return scancode >= 0 && scancode < MAX_SCANCODE && asccode[scancode][0] != 0;
}

static int isLetter(int scancode){
	char ascii = asccode[scancode][0];
	return ascii >= 'a' && ascii <= 'z';
}

static int isSpecialCharacter(int scancode){
    switch(scancode){
        case LSHFT_PRESSED:
        case RSHFT_PRESSED:
            shiftPressed += 1;
            break;
            //como el flag solo dura mientras este apretado
            //cuando suelto la tecla apago el flag
        case LSHFT_RELEASED:
        case RSHFT_RELEASED:
            shiftPressed -= 1;
            break;
        case CAPSLOCK_PRESSED:
            //chequeo solo si está presionada porque luego de ser pulsada
            //se mantiene el flag prendido hasta que vuelva a pulsarse
            capsLockPressed = !capsLockPressed;
            break;
    }
	return 0; //No era un char especial
}

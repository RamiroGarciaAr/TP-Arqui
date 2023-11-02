#include <shell.h>
#include <utils.h>
#include <stdint.h>
#include <testExceptions.h>
#include <pong.h>
#include <snake.h>
#include <syscallsManager.h>


#define EXIT_KEY 27
#define REGS_KEY 29
#define MAXSIZE 15
#define CANTREGS 18
#define CHARHEIGHT 16
#define CHARWIDTH 8
#define MAXINPUTLEN 50

extern int sysGetTicksElapsed();
extern uint64_t * saveRegisters();


typedef struct{
    void (*function)(void);
    char * name;
    char * helpMessage;
}instructionInfo;

instructionInfo instructions[MAXSIZE];
uint64_t *registersValues;

//Cantidad de funciones disponibles
int instructionsCounter = 0;
int isCursorActive = 0;

static void testException0();
static void testException6();
static int readFromUser(char * backup, int maxlen);
static void decodeInput(char * input);
static void setInstructions();
static void setNewInstruction();
static void printRegisters();
static void printCurrentTime();
static void help();
static void uint64ToHex(uint64_t n, char buf[16]);

uint32_t blue = 0x0000FF;
uint32_t red = 0xFF0000;
uint32_t gray = 0x909090;
uint32_t green = 0x00FF00;
uint32_t orange = 0xFFA500;

void openShell(){
    setInstructions();
    setCursorPos(0, getScreenHeight()-CHARHEIGHT);
    printf("Welcome to the shell, here are the instructions available to you: ", gray);
    putchar('\n');
    help();
    char userInput[MAXINPUTLEN];
    print("$> ");
    int result;
    while((result = readFromUser(userInput, MAXINPUTLEN))){
            if (result == MAXINPUTLEN){
                printf("You have reached the limit of characters allowed to be read, here is the list of instructions available:", red);
                putchar('\n');
                help();
            }else {
                decodeInput(userInput);
            }
            print("$> ");
    }
    print("Closing shell, loging out...");
}

static int readFromUser(char * backup, int maxlen){
    char c;
    int len = 0;

    while((len < maxlen - 1) && (c = getchar()) != '\r' ){
        //necesitamos para poner un cursor que se puedan dar dos interrupciones al mismo tiempo
        if(c){
            //si me apretan esc se sale del modo terminal de la shell
            if(c == EXIT_KEY)
                return 0;

            if (c == REGS_KEY){
                registersValues = saveRegisters();
            }
            else if( c != '\b'){
                putchar(c);

                backup[len++] = c;

            } else if(len > 0){
                putchar('\b');
                len--;
            }
        }
    }
    backup[len++] = '\0';
    putchar('\n');
    if (len == maxlen){
        return MAXINPUTLEN; //me pase del maximo de caracteres que se pueden leer
    }
    return 1; //dejo un 1 para indicar que está todo ok
}

static void decodeInput(char * input){
    if (*input == 0){
        return;
    }
    char * inputBuffer[2];
    //obtengo el primer argumento
    int argc;
    argc = strtok(input, ' ', inputBuffer, 2);
    if (argc > 1){
        printf("Too many arguments, remember the available instructions receive no arguments.", red);
        putchar('\n');
        return;
    }
    for(int i = 0; i < instructionsCounter; i++) {
        //si coincide el nombre de la funcion la llamo
        if (strcmp(instructions[i].name, *inputBuffer)) 
        {
            printf("You are accesing: ",red);
            printf(instructions[i].name,green);
            putchar('\n');
            instructions[i].function();
            return;
        }
    }
    print(input);
    printf(" is not a valid instruction. Use the command \'help\' to see all the instructions available.", red);
    putchar('\n');
}


static void setNewInstruction( void (*fn)(void), char * instructionName, char * helpMsg){
    instructions[instructionsCounter].function = fn;
    instructions[instructionsCounter].name = instructionName;
    instructions[instructionsCounter].helpMessage = helpMsg;
    instructionsCounter++;
}

static void setInstructions(){
    //seteamos todas las funciones que se pueden llamar desde la shell
    setNewInstruction(&start_game, "pong", "                   starts the game \'pong\' in full-screen, to close the game press ESC, the keys to control de padles are:\
                                                          Player1: UP: \'w\' | DOWN: \'s\' || Player2: UP: \'i\' | DOWN: \'k\'");
    
    setNewInstruction(&start_snake, "snake", "            runs snake");
    setNewInstruction(&testException0,  "generateException0", "     runs a test to show the exceptionHandler for exception0: zerodivision");
    setNewInstruction(&testException6,  "generateException6", "     runs a test to show the exceptionHandler for exception6: invalidop");
    setNewInstruction(&printRegisters,  "printRegisters", "         before using this function please press CTRL to save the contents from the registers at that moment,\
                                                          then use this instruction to obtain their values");
    setNewInstruction(&printCurrentTime,"printTime", "              prints the current time, using the format: HH:MM:SS");
    setNewInstruction(&help,"help", "                   prints the whole set of instructions available to you with a short description of them");
    //setNewInstruction(&sysZoomIn,"Zoom In","               Zooms into the Screen");
    //setNewInstruction(&sysZoomOut,"Zoom Out","              Zooms out of the Screen");


}

static void help(){
    println("Instructions Available: ");
    for (int i = 0; i < instructionsCounter; i++){
        //antes eran printf con green
        printf(instructions[i].name, orange);
        printf(":", orange);
        printf(instructions[i].helpMessage, green);
        putchar('\n');
    }
    println("Remember that it's case sensitive and none of them has arguments to be passed");
    println("To close the shell press ESC");
}

static void testException0(){
    runDivideByZero();
}

static void testException6(){
    runInvalidOpcode();
}
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

static void printRegisters(){
    static char * registerNames[CANTREGS] = { "RAX: ", "RBX: ", "RCX: ", "RDX: ", "RSI: ", "RDI: ", "RBP: ", "RSP: ", "R8:  ", "R9:  ", "R10: ", "R11: ", "R12: ", "R13: ", "R14: ", "R15: ", "RIP: ", "RFLAGS: "};
    char registerValue[17];
    registerValue[16] = '\0';
    for (int i = 0; i < CANTREGS; i++){
        printf(registerNames[i], blue);
        uint64ToHex(registersValues[i], registerValue);
        print("0x");
        print(registerValue);
        if (i % 4 == 3)
            putchar('\n');
        else
            print("   ");
    }
    putchar('\n');
}

static void printCurrentTime(){
    char currentTime[9];
    getTime(currentTime);
    println(currentTime);
}
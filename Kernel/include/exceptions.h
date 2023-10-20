#ifndef EXCEPTIONS_H_
#define EXCEPTIONS_H_

#define FRAME_HEIGHT 300


const char* registerNames[18] = {
    "RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "RSP", "R8 ", "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "RFLAGS"
};

//mensaje de error comun para todas las excepciones
const char* errorMessage = "The program has been aborted. The following unhandled exception occured: ";

//nombre de cada excepción
const char* exc_0_message = "Division by Zero.";
const char* exc_6_message = "Invalid Opcode.";


#endif
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

//funciones que llevan a que el procesador lance una excepcion del tipo requerido en cada caso
//para el caso de invalidOpcode se utilizo la instruccion especial 'UD2' segun lo encontrado en:
////http://ref.x86asm.net/coder32.html
extern void runDivideByZero();
extern void runInvalidOpcode();

#endif //EXCEPTIONS_H

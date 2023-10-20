#ifndef IRQDISPATCHER_H
#define IRQDISPATCHER_H

//funcion provista por la catedra para el manejo del pic en "Ejemplo IDT" archivo zip
//modificada para agregar el teclado y usar arreglo de punteros a función
void irqDispatcher(uint64_t irq);

#endif //IRQDISPATCHER_H

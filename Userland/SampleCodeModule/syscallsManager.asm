section .text

GLOBAL sysRead
GLOBAL sysReadFromBuffer
GLOBAL sysWrite
GLOBAL sysCustomWrite
GLOBAL sysDrawCustomCharBack
GLOBAL sysSetCursorPos
GLOBAL sysGetTime
GLOBAL sysGetScrWidth
GLOBAL sysGetScrHeight
GLOBAL sysGetTicksElapsed
GLOBAL sysDottedLine
GLOBAL sysDrawFilledRect
GLOBAL sysVideoRefresh
GLOBAL sysClearScreen
GLOBAL sysPlayBeep

;no se pushea rax para mantener el valor de retorno
%macro pushState 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
%endmacro

%macro popState 0
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro makeSysCall 1
    pushState

	mov r9, %1
    ;rdi, rsi, rdx, rcx, r8, r9
    ;r9 es el unico parametro que se acomoda en asm segun el id de la syscall, el resto si tienen contenido se reciben desde C

    int 80h
    popState
	ret
%endmacro
;se reciben las funciones completando solo los parametros que se usan en cada funcion internamente se llenan el resto o se deja basura

sysRead:
	makeSysCall 0

sysReadFromBuffer:
    makeSysCall 1

sysWrite:
	makeSysCall 2

sysCustomWrite:
	makeSysCall 3

sysSetCursorPos:
	makeSysCall 4

sysGetTime:
	makeSysCall 5

sysGetScrWidth:
	makeSysCall 6

sysGetScrHeight:
	makeSysCall 7

sysDrawFilledRect:
	makeSysCall 8

sysDottedLine:
	makeSysCall 9

sysVideoRefresh:
	makeSysCall 10

sysClearScreen:
	makeSysCall 11

sysPlayBeep:
    makeSysCall 12

sysDrawCustomCharBack:
    makeSysCall 13


;provistas por la catedra en "IDT - Ejemplo" archivo zip
GLOBAL _cli
GLOBAL _sti
GLOBAL picMasterMask
GLOBAL picSlaveMask
GLOBAL haltcpu
GLOBAL _hlt

GLOBAL _irq00Handler
GLOBAL _irq01Handler
GLOBAL _irq02Handler
GLOBAL _irq03Handler
GLOBAL _irq04Handler
GLOBAL _irq05Handler

;creadas por nosotros para el manejo de las excepciones manejadas
GLOBAL _exception0Handler
GLOBAL _exception6Handler
;creada por nosotros para el manejo de las system calls
GLOBAL _sysCallRoutine

EXTERN irqDispatcher
EXTERN exceptionDispatcher
EXTERN exceptionsHandler
EXTERN syscallHandler
EXTERN getStackBase

SECTION .text

%macro pushState 0
	push rax
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
	pop rax
%endmacro

;dado por la catedra en pdf IDT- presentacion
%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

;creada por nosotros para manejar excepciones considerando que hay que imprimir el contenido de los registros y volver a la shell de manera segura
%macro exceptionHandlerMaster 1
; Los valores de los registros seran guardados en el vector register_values en el siguiente orden:
; rax, rbx, rcx, rdx, rsi, rdi, rbp, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rip, rflags
	mov [register_values], rax
	mov [register_values+8], rbx
	mov [register_values+16], rcx
	mov [register_values+24], rdx
	mov [register_values+32], rsi
	mov [register_values+40], rdi
	mov [register_values+48], rbp
	mov [register_values+64], r8
	mov [register_values+72], r9
	mov [register_values+80], r10
	mov [register_values+88], r11
	mov [register_values+96], r12
	mov [register_values+104], r13
	mov [register_values+112], r14
	mov [register_values+120], r15

	; Obtenemos el valor que tiene el rsp cuando ocurrio la excepcion sumandole la cantidad de bytes pusheados al valor actual de rsp
	mov rax, rsp
	add rax, 0x28
	mov [register_values+56], rax
	; Obtenemos el valor the rip cuando ocurrio la excepcion con la direccion del return de la interrupcion
	mov rax, [rsp]
	mov [register_values+128], rax
	; Analogo para obtener el valor de rflags
	mov rax, [rsp+8]
	mov [register_values+136], rax

	; Pasaje de parametros por registros para llamar al handler implementado en exceptions.c
	mov rdi, %1
	mov rsi, register_values
	call exceptionsHandler
	;se recupera el contexto anterior reemplazando sobre los valores a popear por iretq
	;https://wiki.osdev.org/Interrupt_Service_Routines
	call getStackBase
    mov qword [rsp+24], rax
    mov qword [rsp], 0x400000
    iretq
%endmacro

%macro exceptionHandler 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call exceptionDispatcher

	popState
	iretq
%endmacro


_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn


;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Zero Division Exception
_exception0Handler:
	exceptionHandlerMaster 00h

;Invalid Opcode exception
_exception6Handler:
	exceptionHandlerMaster 06h

haltcpu:
	cli
	hlt
	ret

;llamado al handler en syscalls.c
_sysCallRoutine:
	call syscallHandler
	iretq


SECTION .bss
	register_values resq 18
section .text

GLOBAL saveRegisters

;reuso el orden de guardado de los registros que se hace para las excepciones
saveRegisters:
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
    	mov rax, register_values
    	ret


SECTION .bss
	register_values resq 18
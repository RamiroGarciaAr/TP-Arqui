GLOBAL isKeyboardPressed
GLOBAL getScanCode
section .text


isKeyboardPressed:
	mov rax, 0
	in al, 64h 	;se chequea si hay alguna tecla para leer desde el puerto 64h del teclado
	and al, 0x01    ;como me interesa solo el ultimo bit hago un and de 0x01
	ret

getScanCode:
	mov rax, 0	;limpio el registro para no alterar el valor en rax tras leer el byte del puerto 60h
	in al, 60h
	ret


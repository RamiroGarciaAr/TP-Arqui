GLOBAL runInvalidOpcode
GLOBAL runDivideByZero

section .text

runInvalidOpcode:
    UD2
	ret

runDivideByZero:
	mov rbx, 0
	div rbx
	ret

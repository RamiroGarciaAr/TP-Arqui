GLOBAL inb_asm
GLOBAL outb_asm

section .text

inb_asm:
    push rbp
    mov rbp, rsp

    mov rdx, rdi
    in al, dx
    
    mov rdx, rdi
    
    mov rsp, rbp
    pop rbp
    ret


outb_asm:
    push rbp
    mov rbp, rsp

    mov rax, rsi
    mov rdx, rdi
    out dx, al
    
    mov rsp, rbp
    pop rbp
    ret





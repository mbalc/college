global _start

section .data
    SYS_WRITE equ 1
    SYS_OPEN equ 1
    SYS_EXIT  equ 60 

    STDOUT    equ 1

    msg   db    "Hello, world!"
    .len   equ  $ - msg

section .text

_start:

    mov rax, SYS_WRITE
    mov rdi, STDOUT
    mov rsi, msg
    mov rdx, 13
    syscall

    mov rax, SYS_EXIT
    mov rdi, 0
    syscall

fail: 
    mov rax, SYS_EXIT
    mov rdi, 1
    syscall

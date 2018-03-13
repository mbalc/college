global _start

section .data
    SYS_READ            equ 0
    SYS_WRITE           equ 1
    SYS_OPEN            equ 2
    SYS_EXIT            equ 60 

    O_RDONLY            equ 0000o
    STDOUT              equ 1
    NEW_LINE            equ 10

    BUFFER_SIZE         equ 1024
    BUCKET_SIZE         equ 256
    EXPECTED_NO_OF_ARGS equ 2

    curr_bckt times BUCKET_SIZE db 0
    last_bckt times BUCKET_SIZE db 0
    ; last_bckt resb BCKT_SIZ

section .bss
    file_buff resb BUFFER_SIZE



section .text

_start:
    cmp dword [rsp], EXPECTED_NO_OF_ARGS
    jne _fail

    ; mov rdi, STDOUT
    ; mov rsi, [rsp+16]  ; argv[1]
    ; mov rdx, 99
    ; mov rax, SYS_WRITE
    ; syscall

    mov rdi, [rsp+16]  ; argv[1]
    mov rsi, O_RDONLY
    mov rax, SYS_OPEN
    syscall

    cmp rax, 0
    jl _fail
    mov r14, rax  ; file descriptor
    xor r15, r15  ; reads counter
    jmp _read_file

_read_file:
    mov rdi, r14
    mov rsi, file_buff
    mov rdx, BUFFER_SIZE
    mov rax, SYS_READ
    syscall

    test rax, rax
    jz _end_of_file

    mov r13, rax  ; size of buffer read
    inc r15
    jmp _analyze_buffer

_end_of_file:
    test r15, r15
    jz _fail
    jmp _end

_analyze_buffer: 
    mov rdi, STDOUT
    mov rsi, file_buff
    mov rdx, r13
    mov rax, SYS_WRITE
    syscall

    jmp _read_file

_push_bckt:

%macro return 1
    mov rax, SYS_EXIT
    mov rdi, %1
    syscall
%endmacro

_fail: return 1
_end: return 0
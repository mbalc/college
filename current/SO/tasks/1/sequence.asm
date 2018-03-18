global _start

section .data
    SYS_READ            equ 0
    SYS_OPEN            equ 2
    SYS_EXIT            equ 60 

    O_RDONLY            equ 0000o
    STDOUT              equ 1
    NEW_LINE            equ 10

    BUFFER_SIZE         equ 8156
    ALPHABET_LIMIT      equ 256
    EXPECTED_NO_OF_ARGS equ 2

    curr_bucket times ALPHABET_LIMIT db 0
    last_bucket times ALPHABET_LIMIT db 0

section .bss
    file_buffer resb BUFFER_SIZE

section .text
_start:
    cmp dword [rsp], EXPECTED_NO_OF_ARGS
    jne _fail

    mov rdi, [rsp+16]  ; argv[1]
    mov rsi, O_RDONLY
    mov rax, SYS_OPEN
    syscall

    cmp rax, 0
    jl _fail
    mov r14, rax  ; file descriptor
    xor r10, r10
    xor r15, r15  ; zero when there's a permutation parsing in progress
    jmp _read_file

_read_file:
    mov rdi, r14
    mov rsi, file_buffer
    mov rdx, BUFFER_SIZE
    mov rax, SYS_READ
    syscall

    test rax, rax
    jz _end_of_file  ; if rax==0 then we have already read everything

    mov r13, rax  ; size of buffer read
    add r13, file_buffer  ; the last character read
    mov r12, file_buffer  ; iterator for file_buffer content
    jmp _analyze_buffer

_end_of_file:
    test r15, r15
    jz _fail  ; we haven't read anything!
    jmp _end

_analyze_buffer: 
    xor r15, r15
    mov r10b, byte [r12]
    mov r8, curr_bucket
    add r8, r10
    cmp byte [r8], 0  ; TODO ask and find "test" alternative
    jnz _fail  ; a number repeated within one permutation

    inc byte [r8]

    test r10b, r10b
    jnz _proceed

    xor rcx, rcx
    mov r8, curr_bucket
    mov r9, last_bucket
    call _handle_zero

    xor rcx, rcx
    mov r8, curr_bucket
    mov r9, last_bucket
    call _copy_buckets

_proceed:
    inc r12
    cmp r12, r13
    jl _analyze_buffer
    jmp _read_file

_handle_zero:
    mov r15, 1  ; indicate that there is no permutation pending
    cmp byte [r9], 0
    jne _checker_loop
    ret

_checker_loop:
    mov r11b, byte [r8]
    cmp r11b, byte [r9]  ; to be fixed
    jne _fail

    inc r8
    inc r9
    inc rcx
    cmp rcx, ALPHABET_LIMIT
    jl _checker_loop
    ret

_copy_buckets:
    mov r11b, byte [r8]
    mov byte [r9], r11b
    mov byte [r8], 0

    inc r8
    inc r9
    inc rcx
    cmp rcx, ALPHABET_LIMIT
    jl _copy_buckets
    ret

%macro return 1
    mov rax, SYS_EXIT
    mov rdi, %1
    syscall
%endmacro

_fail: return 1
_end: return 0
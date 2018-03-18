global _start

section .data
    SYS_READ            equ 0
    SYS_OPEN            equ 2
    SYS_EXIT            equ 60 

    O_RDONLY            equ 0000o
    STDOUT              equ 1
    NEW_LINE            equ 10

    BUFFER_SIZE         equ 8156
    ALPHABET_LIMIT      equ 256  ; number of possible 8bit combinations
    EXPECTED_NO_OF_ARGS equ 2  ; one argument expected (path to the file)

    checklist times ALPHABET_LIMIT db 0
    ; every alphabet element has a corresponding byte in this checklist
    ; during number-by-number analysis 

section .bss
    file_buffer resb BUFFER_SIZE  ; file fragment we analyse at one moment

section .text
_start:
    cmp dword [rsp], EXPECTED_NO_OF_ARGS
    jne _fail  ; wrong number of args

    mov rdi, [rsp+16]  ; argv[1]
    mov rsi, O_RDONLY
    mov rax, SYS_OPEN
    syscall  ; open file and get descriptor

    cmp rax, 0
    jl _fail  ; opening the file ended up with an error

    mov r14, rax  ; r14 - file descriptor
    xor r10, r10
    xor r15, r15  ; r15 - zero when there's a permutation parsing in progress

_read_file:
    mov rdi, r14
    mov rsi, file_buffer
    mov rdx, BUFFER_SIZE
    mov rax, SYS_READ
    syscall  ; read file contents to the buffer

    cmp rax, 0
    je _end_of_file  ; we have already read everything
    jl _fail  ; file reading has failed for some reason

    mov r13, rax  ; size of buffer read
    add r13, file_buffer  ; the last character read
    mov r12, file_buffer  ; r12 - iterator for file_buffer content
    jmp _analyze_buffer

_end_of_file:
    test r15, r15
    jz _fail  ; we haven't read anything!
    jmp _end

_analyze_buffer: 
    xor r15, r15  ; permutation parsing in progress
    mov r10b, byte [r12]
    mov r8, checklist
    add r8, r10
    inc byte [r8]

    cmp byte [r8], 3
    jge _fail  ; a number has definitely repeated
               ; (I avoid counter overflow by doing this check early)

    test r10b, r10b
    jnz _proceed

    xor rcx, rcx
    mov r8, checklist
    call _handle_zero

_proceed:
    inc r12
    cmp r12, r13
    jl _analyze_buffer
    jmp _read_file

_handle_zero:
    mov r15, 1  ; indicate that there is no permutation pending
    mov r11b, byte [r8]

    cmp r11b, 1
    je _first_permutation
    dec r11b
    jmp _other_permutations  ; otherwise r11b has to be equal to 2

_first_permutation:
    cmp byte [r8], 0
    je _loop_1
    cmp r11b, byte [r8]
    je _loop_1
    jne _fail

_other_permutations:
    cmp byte [r8], 0
    je _loop_2
    dec byte [r8]  ; avoid counter ever overflowing
    cmp r11b, byte [r8]
    je _loop_2
    jne _fail

%macro looper 1
    inc r8
    inc rcx
    cmp rcx, ALPHABET_LIMIT
    jl %1
    ret
%endmacro

_loop_1: looper _first_permutation
_loop_2: looper _other_permutations

%macro return 1
    mov rax, SYS_EXIT
    mov rdi, %1
    syscall
%endmacro

_fail: return 1
_end: return 0
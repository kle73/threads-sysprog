.section .text
.global add

add:
    movq %rdi, %rdx
    addq %rsi, %rax
    ret

.data
hello_str : .ascii "Hello World!\n"

.text
.global main
main:
    movl $4, %eax
    movl $1, %ebx
    movl $hello_str, %ecx
    movl $13, %edx
    int $0x80


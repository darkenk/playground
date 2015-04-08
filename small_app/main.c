void _start()
{
    asm("movl $1, %eax\n\t"
        "xorl %ebx, %ebx\n\t"
        "int $0x80");
}

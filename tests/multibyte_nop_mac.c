#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint64_t x = 0x1111111111111111ULL;

    printf("Before asm, x = 0x%llx\n", x);

    __asm__ volatile (
        ".byte 0x0f, 0x1f, 0xc1\n\t"

        ".byte 0x0f, 0x1f, 0x44, 0x00, 0x00\n\t"

        "mov $0x2222222222222222, %0\n\t"
        : "=r"(x)
        :
        : "memory"
    );

    printf("After asm,  x = 0x%llx\n", x);
    return 0;
}

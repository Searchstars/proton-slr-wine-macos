#include <stdio.h>
#include <stdint.h>

int main(void)
{
    uint64_t x = 0x1111111111111111ULL;

    printf("Before asm, x = 0x%llx\n", (unsigned long long)x);

#if defined(__x86_64__) || defined(_M_X64)
    __asm__ __volatile__ (
        ".byte 0x0f, 0x1f, 0xc1\n\t"

        ".byte 0x0f, 0x1f, 0x44, 0x00, 0x00\n\t"

        "movabs $0x2222222222222222, %0\n\t"
        : "=r"(x)
        :
        : "memory"
    );
#else
#   error "This test must be built as x86_64."
#endif

    printf("After asm,  x = 0x%llx\n", (unsigned long long)x);
    return 0;
}

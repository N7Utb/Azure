#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    while (*s)
    {
        sbi_ecall(0x1,0x0,*s,0,0,0,0,0);
        s++;
    }
}

void puti(int x) {
    char s[11];
    char* p = s;
    while (x)
    {
        *p = x % 10 + '0';
        x /= 10;
        p++;
    }
    p--;
    while (p>=s)
    {
        sbi_ecall(0x1,0x0,*p,0,0,0,0,0);
        p--;
    }
}

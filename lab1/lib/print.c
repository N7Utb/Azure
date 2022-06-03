#include "print.h"
#include "sbi.h"

void puts(char *s) {
    // unimplemented
    // output the characters one by one
    for(int i=0; s[i]; i++){
        sbi_ecall(0x1, 0x0, s[i], 0, 0, 0, 0, 0); 
    }
    return ;
}

void puti(int x) {
    // unimplemented
    int stack[16], top = -1;
    // transfrom the decimal digits one by one
    while(x){
        stack[++top] = x%10; 
        x/=10; 
    }  
    // output the digits one by one
    for( ; top>-1; top--){
        sbi_ecall(0x1, 0x0, stack[top]+'0', 0, 0, 0, 0, 0); 
    }
    return ;
}

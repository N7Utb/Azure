#include "printk.h"
#include "defs.h"

// Please do not modify

void test() {
    int i=0; 
    while (1){
        i++; 
        if(i==350000000){
            printk("kernel is running!\n");
            i=0;  
        }
    }
}

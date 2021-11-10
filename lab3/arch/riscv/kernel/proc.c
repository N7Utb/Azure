/*
 * created by N7Utb at 2021/11/10
 */
#include "proc.h"
extern void __dummy();

struct task_struct* idle;
struct task_struct* current;
struct task_struct* task[NR_TASKS];

void task_init(){
    
}
/*
 * created by N7Utb at 2021/11/10
 */
#include "proc.h"
#include "rand.h"
#include "vm.h"
extern char _stext[];
extern char _etext[];
extern char _srodata[];
extern char _erodata[];
extern char _sdata[];
extern char _edata[];
extern char _sbss[];
extern char _ebss[];
extern char _ekernel[];
extern void __dummy();
extern void __switch_to(struct task_struct *prev, struct task_struct *next);
extern char uapp_start[];
extern char uapp_end[];
struct task_struct *idle;
struct task_struct *current;
struct task_struct *task[NR_TASKS];
void dummy()
{
    uint64 MOD = 1000000007;
    uint64 auto_inc_local_var = 0;
    int last_counter = -1;
    while (1)
    {
        if (last_counter == -1 || current->counter != last_counter)
        {
            last_counter = current->counter;
            auto_inc_local_var = (auto_inc_local_var + 1) % MOD;
            printk("[PID = %d] is running. thread space begin at = %lx\n", current->pid,current);
        }
    }
}
void task_init()
{
    unsigned long u_stack = 0;
    idle = (struct task_struct *)kalloc(PGSIZE);

    idle->state = TASK_RUNNING;
    idle->counter = 0;
    idle->priority = 0;
    idle->pid = 0;
    current = idle;
    task[0] = idle;

    for (int i = 1; i < NR_TASKS; i++)
    {
        task[i] = (struct task_struct *)kalloc(PGSIZE);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 0;
        task[i]->priority = rand() % PRIORITY_MAX + PRIORITY_MIN;
        task[i]->pid = i;
        task[i]->thread.ra = (uint64)__dummy;
        task[i]->thread.sp = (uint64)task[i] + PGSIZE; 
        task[i]->thread.sepc = USER_START;
        task[i]->thread.sstatus = SUM_MASK | SPIE_MASK;
        task[i]->thread.sscratch = USER_END;
        task[i]->pgd = (pagetable_t)kalloc(PGSIZE);
        u_stack = (unsigned long)kalloc(PGSIZE);
        printk("task[%d]->pgd = 0x%lx\n",i,task[i]->pgd);
        memset(task[i]->pgd,0x0,PGSIZE);
        create_mapping(task[i]->pgd,(unsigned long)_stext,
                        (unsigned long)(_stext - PA2VA_OFFSET),
                        (unsigned long)PGROUNDUP(_etext - _stext),
                        X_MASK | R_MASK | V_MASK);
        create_mapping(task[i]->pgd,(unsigned long)_srodata,
                        (unsigned long)(_srodata - PA2VA_OFFSET), 
                        (unsigned long)PGROUNDUP(_erodata - _srodata), 
                        R_MASK | V_MASK);
        create_mapping(task[i]->pgd,(unsigned long)_sdata,
                        (unsigned long)(_sdata - PA2VA_OFFSET), 
                        (unsigned long)PGROUNDUP(_edata - _sdata), 
                        W_MASK | R_MASK | V_MASK);
        create_mapping(task[i]->pgd,(unsigned long)_sbss,
                        (unsigned long)(_sbss - PA2VA_OFFSET), 
                        (unsigned long)PGROUNDUP(_ebss - _sbss), 
                        W_MASK | R_MASK | V_MASK);

        create_mapping(task[i]->pgd,(unsigned long)PGROUNDUP(_ekernel), 
                        (unsigned long)PGROUNDUP(_ekernel - PA2VA_OFFSET),
                        (unsigned long)( PHY_END -  PGROUNDUP(_ekernel - PA2VA_OFFSET)),
                        W_MASK | R_MASK | V_MASK);       

        create_mapping(task[i]->pgd,
                       (unsigned long)USER_START,
                       (unsigned long)(uapp_start - PA2VA_OFFSET),
                       (unsigned long)PGROUNDUP(uapp_end - uapp_start),
                       U_MASK | X_MASK  | R_MASK | V_MASK | W_MASK);
        create_mapping(task[i]->pgd,
                       (unsigned long)(USER_END - PGSIZE),
                       (unsigned long)(u_stack - PA2VA_OFFSET - PGSIZE),
                       (unsigned long)PGSIZE,
                       U_MASK  | R_MASK | V_MASK | W_MASK);
    }

    printk("...proc_init done!%d\n", NR_TASKS);
}

void switch_to(struct task_struct *next)
{
    struct task_struct *p = current;
    if (current == next)
    {
        return;
    }
    else
    {
        current = next;
        // printk("\nswitch to [PID = %d COUNTER = %d]\n",current->pid, current->counter);
        __switch_to(p, next);

        return;
    }
}
// SJF
#ifdef __schedule_SJF
void schedule(void)
{
    struct task_struct *p = task[1];
    int zero_flag = 0, i;
    for (i = 1; i < NR_TASKS; i++)
    {
        if (task[i]->counter)
        {
            break;
        }
    }
    if (i == NR_TASKS)
    {
        for (i = 1; i < NR_TASKS; i++)
        {

            task[i]->counter = rand() % 10 + 1;
            // printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n", i, task[i]->priority, task[i]->counter);
        }
    }
    for (i = 2; i < NR_TASKS; i++)
    {
        if (p->counter == 0 || p->state != TASK_RUNNING)
            p = task[i];
        else if (task[i]->counter < p->counter && task[i]->state == TASK_RUNNING && task[i]->counter != 0)
            p = task[i];
    }
    switch_to(p);
    return;
}
#else
void schedule(void)
{
    struct task_struct *p = task[1];
    int zero_flag = 0, i;
    for (i = 1; i < NR_TASKS; i++)
    {
        if (task[i]->counter)
        {
            break;
        }
    }
    if (i == NR_TASKS)
    {
        for (i = 1; i < NR_TASKS; i++)
        {
            task[i]->counter = rand() % 10 + 1;
            // printk("task[%d]->counter = %d  ", i, task[i]->counter);
            // printk("task[%d]->priority = %d\n", i, task[i]->priority);
            printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n", i, task[i]->priority, task[i]->counter);
        }
    }
    for (i = 2; i < NR_TASKS; i++)
    {
        if (p->counter == 0 || p->state != TASK_RUNNING)
            p = task[i];
        else if (task[i]->priority >= p->priority && task[i]->state == TASK_RUNNING && task[i]->counter != 0)
            p = task[i];
    }

    switch_to(p);
    return;
}
#endif
// // TODO: priority schedule
void do_timer(void)
{
    /* 1. 如果当前线程是 idle 线程 直接进⾏调度 */
    /* 2. 如果当前线程不是 idle 对当前线程的运⾏剩余时间减 1
若剩余时间任然⼤于0 则直接返回 否则进⾏调度 */
    /* YOUR CODE HERE */
    if (current == idle)
    {
        schedule();
        return;
    }
    else
    {
        if (--current->counter)
            return;
        else
        {
            schedule();
            return;
        }
    }
}
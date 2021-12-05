/*
 * created by N7Utb at 2021/11/10
 */
#include "proc.h"
#include "mm.h"
#include "rand.h"
#include "printk.h"
#include "defs.h"
extern void __dummy();
extern void __switch_to(struct task_struct *prev, struct task_struct *next);

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
            printk("[PID = %d] is running. auto_inc_local_var = %d\n", current->pid,
                   auto_inc_local_var);
        }
    }
}
void task_init()
{
    // 1. 调⽤ kalloc() 为 idle 分配⼀个物理⻚
    // 2. 设置 state 为 TASK_RUNNING;
    // 3. 由于 idle 不参与调度 可以将其 counter / priority 设置为 0
    // 4. 设置 idle 的 pid 为 0
    // 5. 将 current 和 task[0] 指向 idle
    /* YOUR CODE HERE */
    idle = (struct task_struct *)kalloc(PGSIZE);

    idle->state = TASK_RUNNING;
    idle->counter = 0;
    idle->priority = 0;
    idle->pid = 0;
    current = idle;
    task[0] = idle;
    // 1. 参考 idle 的设置, 为 task[1] ~ task[NR_TASKS - 1] 进⾏初始化
    // 2. 其中每个线程的 state 为 TASK_RUNNING, counter 为 0, priority 使⽤ rand() 来设置,pid 为该线程在线程数组中的下标。
    // 3. 为 task[1] ~ task[NR_TASKS - 1] 设置 `thread_struct` 中的 `ra` 和 `sp`,
    // 4. 其中 `ra` 设置为 __dummy （⻅ 4.3.2）的地址， `sp` 设置为 该线程申请的物理⻚的⾼地址
    /* YOUR CODE HERE */
    for (int i = 1; i < NR_TASKS; i++)
    {
        task[i] = (struct task_struct *)kalloc(PGSIZE);
        task[i]->state = TASK_RUNNING;
        task[i]->counter = 0;
        task[i]->priority = rand() % PRIORITY_MAX + PRIORITY_MIN;
        task[i]->pid = i;
        task[i]->thread.ra = (uint64)__dummy;
        task[i]->thread.sp = (uint64)task[i] + PGSIZE;
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
            // printk("task[%d]->counter = %d  ", i, task[i]->counter);
            // printk("task[%d]->priority = %d\n", i, task[i]->priority);
            printk("SET [PID = %d PRIORITY = %d COUNTER = %d]\n", i, task[i]->priority, task[i]->counter);
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
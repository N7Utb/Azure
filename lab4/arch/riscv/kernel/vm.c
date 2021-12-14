#include "defs.h"
#include "string.h"
#include "mm.h"
#include "printk.h"
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));
extern char _stext[];
extern char _etext[];
extern char _srodata[];
extern char _erodata[];
extern char _sdata[];
extern char _edata[];
extern char _sbss[];
extern char _ebss[];
extern char _ekernel[];
void setup_vm(void)
{
    early_pgtbl[VM_START >> 30 & 0x1ff] = ((PHY_START >> 30 & 0x3ffffff) << 28) + (V_MASK | R_MASK | W_MASK | X_MASK);
    // early_pgtbl[PHY_START >> 30 & 0x1ff] = ((PHY_START >> 30 & 0x3ffffff) << 28) + (V_MASK | R_MASK | W_MASK | X_MASK);
    printk("before: 0x%lx\n", early_pgtbl); 
    // TODO: find a solution that don't need Equivalent mapping
    // ! no hurry
    // *: The purpose for Equivalent mapping is that, the first call to `csrw satp` the context is still under the physical address space(0x8000000)
    // *: After the set, the context is switch to the virtual address space, but rip is still 0x800000
    // *: thus,If we don't set the equivalent mapping, the rip will lost itself.
    return;
}
unsigned long swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));
void create_mapping(unsigned long* pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm)
{
    // TODO: setup the three level page table map
    int occu = 0; 
    // !: sz may be over a pagesize
    for(int i=0; sz>0; i++){
        int VPN[3]; 
        unsigned long *addr2, *addr3; 
        unsigned long cur_va = va + i*PGSIZE; 
        // !: one va however multiple pa?
        VPN[0] = (cur_va) >> 12 & 0x1ff; 
        VPN[1] = (cur_va) >> 21 & 0x1ff; 
        VPN[2] = (cur_va) >> 30 & 0x1ff; 

        // first level
        if(pgtbl[VPN[2]] & V_MASK)
        {
            //!: physical?
            addr2 = (unsigned long)((pgtbl[VPN[2]] >> 10) << 12) + PA2VA_OFFSET; 
        }
        else
        {
            addr2 = kalloc(); // !: ? 
            pgtbl[VPN[2]] = ((((unsigned long)addr2 - PA2VA_OFFSET) >> 12) << 10) | V_MASK; 
        }
        // second level
        if(addr2[VPN[1]] & V_MASK)
        {
            //!: physical?
            addr3 = (unsigned long)((addr2[VPN[1]] >> 10) << 12) + PA2VA_OFFSET; 
        }
        else
        {
            addr3 = kalloc(); // !: ?
            addr2[VPN[1]] = ((((unsigned long)addr3 - PA2VA_OFFSET) >> 12) << 10) | V_MASK; 
        }
        // third level
        addr3[VPN[0]] = (((pa >> 12) + i) << 10) | perm; 
        if(va == 0xffffffe000200000 || va == 0xffffffe000202000)
            printk("last address: %lx\n", &addr3[VPN[0]]); 
        occu++; 
        // map next page
        sz -= PGSIZE; 
    }
    printk("%lx ^^ %x ^^ done\n", va, occu); 
}
void setup_vm_final(void){
    printk("after: 0x%lx\n", early_pgtbl); 
    memset(swapper_pg_dir,0x0,PGSIZE);
    create_mapping(swapper_pg_dir,(unsigned long)_stext,
                    (unsigned long)(_stext - PA2VA_OFFSET),
                    (unsigned long)PGROUNDUP(_etext - _stext),
                    X_MASK | R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_srodata,
                    (unsigned long)(_srodata - PA2VA_OFFSET), 
                    (unsigned long)PGROUNDUP(_erodata - _srodata), 
                    R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_sdata,
                    (unsigned long)(_sdata - PA2VA_OFFSET), 
                    (unsigned long)PGROUNDUP(_edata - _sdata), 
                    W_MASK | R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_sbss,
                    (unsigned long)(_sbss - PA2VA_OFFSET), 
                    (unsigned long)PGROUNDUP(_ebss - _sbss), 
                    W_MASK | R_MASK | V_MASK);

    create_mapping(swapper_pg_dir,(unsigned long)PGROUNDUP(_ekernel), 
                    (unsigned long)PGROUNDUP(_ekernel - PA2VA_OFFSET),
                    (unsigned long)( PHY_END -  PGROUNDUP(_ekernel - PA2VA_OFFSET)),
                    W_MASK | R_MASK | V_MASK);

    printk("tete: %lx\n", swapper_pg_dir);
    // set the stap register and clear tlb
    // !: swapper_pgtbl is virtual?
    __asm__ volatile(
        // mode
        "li a0, 0x8\n"
        "slli a0, a0, 60\n"
        // PPN
        "addi a1, %[PPN], 0\n"
        "srli a1, a1, 12\n"
        "add a0, a0, a1\n"
        "csrw satp, a0\n"
        "sfence.vma zero, zero\n"
        :   // output
        : [PPN] "r" ((unsigned long)swapper_pg_dir-PA2VA_OFFSET)   // input
        : "a0", "a1"   // modification
        );
    printk("*********************\n"); 
    
    return;
}

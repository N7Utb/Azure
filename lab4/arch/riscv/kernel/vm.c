#include "defs.h"
#include "string.h"
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
    early_pgtbl[PHY_START >> 30 & 0x1ff] = ((PHY_START >> 30 & 0x3ffffff) << 28) + (V_MASK | R_MASK | W_MASK | X_MASK);
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
    // !: sz may be over a pagesize
}
void setup_vm_final(void){
    memset(swapper_pg_dir,0x0,PGSIZE);
    create_mapping(swapper_pg_dir,(unsigned long)_stext,(unsigned long)(PHY_START + OPENSBI_SIZE),(unsigned long)PGSIZE,X_MASK | R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_srodata,(unsigned long)(PHY_START + OPENSBI_SIZE + PGSIZE), (unsigned long)PGSIZE, R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_sdata,(unsigned long)(PHY_START + OPENSBI_SIZE + PGSIZE * 2), (unsigned long)PGSIZE, W_MASK | R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_sbss,(unsigned long)(PHY_START + OPENSBI_SIZE + PGSIZE * 3), (unsigned long)PGSIZE, W_MASK | R_MASK | V_MASK);
    create_mapping(swapper_pg_dir,(unsigned long)_ekernel, (unsigned long)(PHY_START + OPENSBI_SIZE + PGSIZE * 4),(unsigned long)(PHY_SIZE - 4*PGSIZE),W_MASK | R_MASK | V_MASK);


    // TODO: use inline asm, set the stap register


    __asm__ volatile("sfence.vma zero, zero");

    return;
}

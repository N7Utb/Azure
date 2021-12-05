#include "defs.h"
#include "string.h"
unsigned long  early_pgtbl[512] __attribute__((__aligned__(0x1000)));
void setup_vm(void)
{
    early_pgtbl[VM_START >> 30 & 0x1ff] = ((PHY_START >> 30 & 0x3ffffff) << 28) + MASK;
    early_pgtbl[PHY_START >> 30 & 0x1ff] = ((PHY_START >> 30 & 0x3ffffff) << 28) + MASK;
    return;
}
unsigned long swapper_pg_dir[512] __attribute__((__aligned__(0x1000)));
void setup_vm_final(void){
    memset(swapper_pg_dir,0x0,PGSIZE);
    
}
void create_mapping(unsigned long* pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm)
{

}
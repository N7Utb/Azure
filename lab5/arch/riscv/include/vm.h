#include "defs.h"
#include "string.h"
#include "mm.h"
#include "printk.h"

void create_mapping(unsigned long* pgtbl, unsigned long va, unsigned long pa, unsigned long sz, int perm);
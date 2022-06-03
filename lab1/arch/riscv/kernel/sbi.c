#include "types.h"
#include "sbi.h"

struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
						uint64 arg1, uint64 arg2,
						uint64 arg3, uint64 arg4,
						uint64 arg5)
{
	// unimplemented
	struct sbiret res; 
	__asm__ volatile(
		// simply assign values to corresponding registers
		"addi a0, %[arg0], 0\n"
		"addi a1, %[arg1], 0\n"
		"addi a2, %[arg2], 0\n"
		"addi a3, %[arg3], 0\n"
		"addi a4, %[arg4], 0\n"
		"addi a5, %[arg5], 0\n"
		"addi a6, %[fid], 0\n"
		"addi a7, %[ext], 0\n"
		"ecall\n"
		// get the result returned
		"addi %[err], a0, 0\n"
		"addi %[val], a1, 0"
		: [err] "=r"(res.error), [val] "=r"(res.value)				// output
		: [ext] "r" (ext), [fid] "r" (fid), [arg0] "r" (arg0), [arg1] "r" (arg1), [arg2] "r" (arg2), [arg3] "r" (arg3), [arg4] "r" (arg4), [arg5] "r" (arg5)	// input
		: "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"	// necessary, or the result may be incorrect
	);
	return res; 
}

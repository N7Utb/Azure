#include "types.h"
#include "sbi.h"


struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
			            uint64 arg1, uint64 arg2,
			            uint64 arg3, uint64 arg4,
			            uint64 arg5) 
{
	// struct sbiret ret;
	// __asm__ volatile(
	// 	// "mv a7, %[ext]\n"
	// 	// "mv a6, %[fid]\n"
	// 	// "mv a0, %[arg0]\n"
	// 	// "mv a1, %[arg1]\n"
	// 	// "mv a2, %[arg2]\n"
	// 	// "mv a3, %[arg3]\n"
	// 	// "mv a4, %[arg4]\n"
	// 	// "mv a5, %[arg5]\n"
	// 	"ecall\n"
	// 	"mv %[ret_err], a0\n"
	// 	"mv %[ret_val], a1\n"
	// 	: [ret_err]"=r" (ret.error ), [ret_val]"=r" (ret.value )
	// 	:  [arg0]"r" (arg0), [arg1]"r" (arg1), [arg2]"r"(arg2), [arg3]"r"(arg3), [arg4]"r"(arg4),[arg5]"r"(arg5), [fid]"r" (fid),[ext]"r" (ext)
	// 	: "memory"

	// );
	// return ret;
	struct sbiret res; 
	__asm__ volatile(
		"addi sp, sp, -16\n"
		"sd   %[ext], 0(sp)\n"	// a7, in fact a0
		"sd   %[fid], 8(sp)\n"	// a6, in fact a1
		"addi a0, %[arg0], 0\n"
		"addi a1, %[arg1], 0\n"
		"addi a2, %[arg2], 0\n"
		"addi a3, %[arg3], 0\n"
		"addi a4, %[arg4], 0\n"
		"addi a5, %[arg5], 0\n"
		"ld   a6, 8(sp)\n"
		"ld   a7, 0(sp)\n"
		"ecall\n"
		"addi %[err], a0, 0\n"
		"addi %[val], a1, 0\n"
		"addi sp, sp, 16\n"
		: [err] "=r"(res.error), [val] "=r"(res.value)				// output
		: [ext] "r" (ext), [fid] "r" (fid), [arg0] "r" (arg0), [arg1] "r" (arg1), [arg2] "r" (arg2), [arg3] "r" (arg3), [arg4] "r" (arg4), [arg5] "r" (arg5)	// input
	);
	return res; 
}
// #include "types.h"
// #include "sbi.h"

// struct sbiret sbi_ecall(int ext, int fid, uint64 arg0,
// 						uint64 arg1, uint64 arg2,
// 						uint64 arg3, uint64 arg4,
// 						uint64 arg5)
// {
// 	// unimplemented
// 	struct sbiret res; 
// 	__asm__ volatile(
// 		"addi a0, %[arg0], 0\n"
// 		"addi a1, %[arg1], 0\n"
// 		"addi a2, %[arg2], 0\n"
// 		"addi a3, %[arg3], 0\n"
// 		"addi a4, %[arg4], 0\n"
// 		"addi a5, %[arg5], 0\n"
// 		"addi a6, %[fid], 0\n"
// 		"addi a7, %[ext], 0\n"
// 		"ecall\n"
// 		"addi %[err], a0, 0\n"
// 		"addi %[val], a1, 0"
// 		: [err] "=r"(res.error), [val] "=r"(res.value)				// output
// 		: [arg0] "r" (arg0), [arg1] "r" (arg1), [arg2] "r" (arg2), [arg3] "r" (arg3), [arg4] "r" (arg4), [arg5] "r" (arg5), [fid] "r" (fid), [ext] "r" (ext)	// input
// 		// a0					a1					a2					a3					a4					a5					a6					a7
// 		// : "a0", "a1", "a2", "a3", "a4", "a5", "a6", "a7"
// 	);
// 	return res; 
// }

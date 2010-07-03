#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <stdint.h>

/*	Execption Handler	*/

extern void _cdecl execption_init();

/* Divide by zero	*/
extern void _cdecl divide_by_zero_fault(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Single step	*/
extern void _cdecl single_step_trap(uint32_t cs,uint32_t eip,uint32_t eflags);

/* No Maskable interrupt trap */
extern void _cdecl nmi_trap(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Breakpoint hit */
extern void _cdecl breakpoint_trap(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Overflow trap */
extern void _cdecl overflow_trap(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Bounds check */
extern void _cdecl bounds_check_fault(uint32_t cs,uint32_t eip,uint32_t eflags);

/* invalid opcode instruction */
extern void _cdecl invalid_opcode_fault(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Device not available */
extern void _cdecl no_device_fault(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Double Fault */
extern void _cdecl double_fault_abort(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* Invalid TSS */
extern void _cdecl invalid_tss_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* Segment not present */
extern void _cdecl no_segment_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* Stack fault */
extern void _cdecl stack_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* General Protection Fault */
extern void _cdecl general_protection_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* Page Fault */
extern void _cdecl page_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* FPU error */
extern void _cdecl fpu_fault(uint32_t cs,uint32_t eip,uint32_t eflags);

/* Alignment Check */
extern void _cdecl alignment_check_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags);

/* Machine Check */
extern void _cdecl machine_check_abort(uint32_t cs,uint32_t eip,uint32_t eflags);

/* FPU Single Instruction Multiple Data (SIMD) error */
extern void _cdecl simd_fpu_fault(uint32_t cs,uint32_t eip,uint32_t eflags);

#endif // EXECPTION_H
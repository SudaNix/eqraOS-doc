#include <hal.h>
#include "exception.h"

extern void _cdecl kernel_panic(const char* msg,...);

#pragma warning (disable:4100)


/*	Execption Handler	*/

/* Divide by zero	*/
void _cdecl divide_by_zero_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Divide by 0");
	for (;;);
}

/* Single step	*/
void _cdecl single_step_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Single step");
	for (;;);
}

/* No Maskable interrupt trap */
void _cdecl nmi_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("NMI trap");
	for (;;);
}

/* Breakpoint hit */
void _cdecl breakpoint_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Breakpoint trap");
	for (;;);
}

/* Overflow trap */
void _cdecl overflow_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Overflow trap");
	for (;;);
}

/* Bounds check */
void _cdecl bounds_check_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Bounds check fault");
	for (;;);
}

/* invalid opcode instruction */
void _cdecl invalid_opcode_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Invalid opcode");
	for (;;);
}

/* Device not available */
void _cdecl no_device_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Device not found");
	for (;;);
}

/* Double Fault */
void _cdecl double_fault_abort(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("Double fault");
	for (;;);
}

/* Invalid TSS */
void _cdecl invalid_tss_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("Invalid TSS");
	for (;;);
}

/* Segment not present */
void _cdecl no_segment_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("Invalid segment");
	for (;;);
}

/* Stack fault */
void _cdecl stack_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("Stack fault");
	for (;;);
}

/* General Protection Fault */
void _cdecl general_protection_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("General Protection Fault");
	for (;;);
}


/* Page Fault */
void _cdecl page_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("Page Fault");
	for (;;);
}

/* FPU error */
void _cdecl fpu_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("FPU Fault");
	for (;;);
}

/* Alignment Check */
void _cdecl alignment_check_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	kernel_panic("Alignment Check");
	for (;;);
}

/* Machine Check */
void _cdecl machine_check_abort(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Machine Check");
	for (;;);
}

/* FPU Single Instruction Multiple Data (SIMD) error */
void _cdecl simd_fpu_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("FPU SIMD fault");
	for (;;);
}


#include <hal.h>
#include "exception.h"

extern void _cdecl kernel_panic(const char* msg,...);

#define intstart() \
	_asm	cli \
	_asm	sub		ebp, 4


#pragma warning (disable:4100)


/*	Execption Handler	*/

/* Set Execption */
void _cdecl execption_init() {
	set_vector(0,(void (_cdecl &)(void))divide_by_zero_fault);
	set_vector(1,(void (_cdecl &)(void))single_step_trap);
	set_vector(2,(void (_cdecl &)(void))nmi_trap);
	set_vector(3,(void (_cdecl &)(void))breakpoint_trap);
	set_vector(4,(void (_cdecl &)(void))overflow_trap);
	set_vector(5,(void (_cdecl &)(void))bounds_check_fault);
	set_vector(6,(void (_cdecl &)(void))invalid_opcode_fault);
	set_vector(7,(void (_cdecl &)(void))no_device_fault);
	set_vector(8,(void (_cdecl &)(void))double_fault_abort);
	set_vector(10,(void (_cdecl &)(void))invalid_tss_fault);
	set_vector(11,(void (_cdecl &)(void))no_segment_fault);
	set_vector(12,(void (_cdecl &)(void))stack_fault);
	set_vector(13,(void (_cdecl &)(void))general_protection_fault);
	set_vector(14,(void (_cdecl &)(void))page_fault);
	set_vector(16,(void (_cdecl &)(void))fpu_fault);
	set_vector(17,(void (_cdecl &)(void))alignment_check_fault);
	set_vector(18,(void (_cdecl &)(void))machine_check_abort);
	set_vector(19,(void (_cdecl &)(void))simd_fpu_fault);
}


/* Divide by zero	*/
void _cdecl divide_by_zero_fault(uint32_t cs,uint32_t eip,uint32_t eflags,uint32_t other) {
	
	_asm {
		cli
		add esp, 12
		pushad
	}
	
	kernel_panic("Divide by 0 at physical address [0x%x:0x%x] EFLAGS [0x%x] other: 0x%x",cs,eip, eflags, other);
	for (;;);
}

/* Single step	*/
void interrupt _cdecl single_step_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	kernel_panic("Single step");
	for (;;);
}

/* No Maskable interrupt trap */
void interrupt _cdecl nmi_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("NMI trap");
	for (;;);
}

/* Breakpoint hit */
void interrupt _cdecl breakpoint_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Breakpoint trap");
	for (;;);
}

/* Overflow trap */
void interrupt _cdecl overflow_trap(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Overflow trap");
	for (;;);
}

/* Bounds check */
void interrupt _cdecl bounds_check_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Bounds check fault");
	for (;;);
}

/* invalid opcode instruction */
void interrupt _cdecl invalid_opcode_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Invalid opcode");
	for (;;);
}

/* Device not available */
void interrupt _cdecl no_device_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Device not found");
	for (;;);
}

/* Double Fault */
void interrupt _cdecl double_fault_abort(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Double fault");
	for (;;);
}

/* Invalid TSS */
void interrupt _cdecl invalid_tss_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Invalid TSS");
	for (;;);
}

/* Segment not present */
void interrupt _cdecl no_segment_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Invalid segment");
	for (;;);
}

/* Stack fault */
void interrupt _cdecl stack_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Stack fault");
	for (;;);
}

/* General Protection Fault */
void interrupt _cdecl general_protection_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("General Protection Fault");
	for (;;);
}


/* Page Fault */
void interrupt _cdecl page_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	
	_asm {
		cli
		sub ebp,4
		popad
		sti
		iretd
	}
	
	//kernel_panic("Page Fault");
	//for (;;);
}

/* FPU error */
void interrupt _cdecl fpu_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("FPU Fault");
	for (;;);
}

/* Alignment Check */
void interrupt _cdecl alignment_check_fault(uint32_t cs,uint32_t err,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Alignment Check");
	for (;;);
}

/* Machine Check */
void interrupt _cdecl machine_check_abort(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("Machine Check");
	for (;;);
}

/* FPU Single Instruction Multiple Data (SIMD) error */
void interrupt _cdecl simd_fpu_fault(uint32_t cs,uint32_t eip,uint32_t eflags) {
	intstart ();
	kernel_panic("FPU SIMD fault");
	for (;;);
}


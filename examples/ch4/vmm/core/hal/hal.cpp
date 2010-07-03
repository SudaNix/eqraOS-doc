

#ifndef i386
#error "[hal.cpp] is not implemented in this platform"
#endif

#include <hal.h>
#include "cpu.h"
#include "idt.h"
#include "pic.h"
#include "pit.h"

int _cdecl hal_init() {
	i386_cpu_init();
	i386_pic_init(0x20,0x28);
	i386_pit_init();
	i386_pit_start_counter(100,I386_PIT_OCW_COUNTER_0,I386_PIT_OCW_MODE_SQUAREWAVEGEN);
	
	/*	enable irq	*/
	enable_irq();
	
	return 0;
}

int _cdecl hal_close() {
	i386_cpu_close();
	return 0;
}

void _cdecl gen_interrupt(int n) {
#ifdef _MSC_VER
	_asm {
		mov al, byte ptr [n]
		mov byte ptr [address+1], al
		jmp address
		
		address:
			int 0		// will execute int n.
	}
#endif
}


void _cdecl int_done(unsigned int n) {
	if (n > 16)
		return;
		
	if (n > 7)
		/* send EOI to pic2	*/
		i386_pic_send_command(I386_PIC_OCW2_MASK_EOI,1);
		
	/* also send toi the primary pic */
	i386_pic_send_command(I386_PIC_OCW2_MASK_EOI,0);
}

void _cdecl sound(unsigned int f) {
	outportb(0x61,3 | unsigned char(f << 2));
}

unsigned char _cdecl inportb(unsigned short port_num) {
#ifdef _MSC_VER
	_asm {
		mov dx,word ptr [port_num]
		in al,dx
		mov byte ptr [port_num],al
	}
#endif

	return unsigned char(port_num);
}


void _cdecl outportb(unsigned short port_num,unsigned char value) {
#ifdef _MSC_VER
	_asm {
		mov al,byte ptr[value]
		mov dx,word ptr[port_num]
		out dx,al
	}
#endif
}

void _cdecl enable_irq() {
#ifdef _MSC_VER
	_asm sti
#endif
}

void _cdecl disable_irq() {
#ifdef _MSC_VER
	_asm cli
#endif
}

void _cdecl set_vector(unsigned int int_num,void (_cdecl far & vect)()) {
	i386_idt_install_ir(int_num,I386_IDT_32BIT|I386_IDT_PRESENT /*10001110*/,0x8 /*code desc*/,vect);
}

void (_cdecl far * _cdecl get_vector(unsigned int int_num))() {
	idt_desc* desc = i386_get_idt_ir(int_num);
	
	if (desc == 0)
		return 0;
		
	uint32_t address = desc->base_low | (desc->base_high << 16);
	
	I386_IRQ_HANDLER irq = (I386_IRQ_HANDLER) address;
	return irq;
}

const char* _cdecl get_cpu_vendor() {
	return i386_cpu_vendor();
}

int _cdecl get_tick_count() {
	return i386_pit_get_tick_count();
}
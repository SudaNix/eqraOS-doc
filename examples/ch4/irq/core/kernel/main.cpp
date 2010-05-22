/*
 Kernel main function.

 eqraOS project,Copyright (c) 2010 Ahmad Essam.
			suda[DOT]nix[AT]hotmail[DOT]com

 This program is free software; you can redistribute it and/or modify
 it under the terms of version 2 of the GNU General Public License
 as published by the Free Software Foundation.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License along
 with this program; if not, write to the Free Software Foundation, Inc.,
 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <hal.h>
#include "kdisplay.h"
#include "exception.h"

/* foreground color on background color	*/

#define WHITE_ON_BLUE	0x1f
#define WHITE_ON_RED	0x4f
#define WHITE_ON_GREEN	0x2f
#define WHITE_ON_BROWN	0x6f

#define GRAY_ON_BLUE	0x17
#define GRAY_ON_RED		0x47
#define GRAY_ON_GREEN	0x27
#define GRAY_ON_BROWN	0x67

#define BLACK_ON_GRAY	0x70
#define BLACK_ON_RED	0x40
#define BLACK_ON_GREEN	0x20
#define BLACK_ON_BROWN	0x60



int _cdecl main()
{
	/*kclear(0x18);		//blue
	kgoto_xy(0,0);

	kset_color(0x70);  //black on gray
	kputs("eqra Operating System (eqraOS) Preparing to load...");

	kgoto_xy(0,1);
	kset_color(0x19);
	kputs("eqraOS Starting Up...");

	kset_color(0x70);
	kgoto_xy(0,24);
	kputs(" Initializing Hardware Abstraction Layer (HAL.lib)... "); 

	kset_color(0x19);
	kgoto_xy(0,2);
	*/
	
	hal_init();
	enable_irq();
	
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
	
	kclear(WHITE_ON_RED);
	kgoto_xy(0,0);
	
	kset_color(BLACK_ON_RED);
	kprintf("    eqraOS Kernel executed.\n\n");

	kset_color(GRAY_ON_RED);

	kprintf("                ___ ___ ________ _  / __ \\ / __/\n");
	kprintf("               / -_) _ `/ __/ _ `/ / /_/ /_\\ \\  \n");
	kprintf("               \\__/\\_, /_/  \\_,_/  \\____//___/  \n");
	kprintf("                    /_/                         \n");
	

	kset_color(WHITE_ON_RED);

	kprintf("Entering PMode ....................................[ok]\n");
	kprintf("Initializing CRT ..................................[ok]\n");
	kprintf("Initializing GDT ..................................[ok]\n");
	kprintf("Initializing IDT ..................................[ok]\n");
	kprintf("Initializing PIC ..................................[ok]\n");
	kprintf("Initializing PIT ..................................[ok]\n");
	kprintf("Setup Error/Exception Handler .....................[ok]\n");
	
	//kprintf("CPU vendor is: %s\n",get_cpu_vendor());
	
	kset_color(BLACK_ON_GRAY);


	for (;;) {
		kgoto_xy(0,24);
		kprintf("Current tick count: %d\n",get_tick_count());
	}
	
	
	return 0;
}
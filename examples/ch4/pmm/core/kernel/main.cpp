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
#include <multiboot_info.h>

#include "kdisplay.h"
#include "exception.h"
#include "pmm.h"
#include "mem_info.h"
#include "color.h"

void eqraos_screen();
void pmm_demo();



int _cdecl main(multiboot_info* boot_info)
{
	mem_info_init(boot_info);

	eqraos_screen();

	kset_color(GRAY_ON_BLUE);
	kputs("eqraOS kernel executed\n");

	mem_info_dump_size();
	mem_info_dump_region();
	hal_init();
	enable_irq();
	execption_init();
	mem_info_blocks_stat();

	// allocate and deallocate demos.
	pmm_demo();
	
	return 0;
}


void pmm_demo() {
	
	kset_color(0x12);
	
	uint32_t* b1 = (uint32_t*)pmm_alloc();
	uint32_t* b2 = (uint32_t*)pmm_allocs(2);
	
	kprintf("b1 allocataed 1 block at 0x%x: \n",b1);
	kprintf("b2 allocataed 2 blocks at 0x%x: \n",b2);

	pmm_dealloc(b1);
	b1 = (uint32_t*)pmm_alloc();
	kprintf("b1 re-allocataed at 0x%x: \n",b1);
	
	pmm_dealloc(b1);
	pmm_deallocs(b2,2);
}

void eqraos_screen() {
	kclear(0x13);
	kgoto_xy(0,0);
	kset_color(0x3F);
	kputs("                                  eqraOS v0.1                                   ");
	//kgoto_xy(0,1);
	//kputs("                          Ahmad Essam [suda.nix@hotmail.com]                    ");
	
	kgoto_xy(0,24);
	kset_color(0x3F);
	kputs("                                                                                ");
	
	kgoto_xy(0,3);
}
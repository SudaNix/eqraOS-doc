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

int _cdecl main()
{
	kclear(0x18);
	kgoto_xy(0,0);

	kset_color(0x70);
	kputs("eqra Operating System (eqraOS) Preparing to load...");

	kgoto_xy(0,1);
	kset_color(0x19);
	kputs("eqraOS Starting Up...");

	kset_color(0x70);
	kgoto_xy(0,24);
	kputs(" Initializing Hardware Abstraction Layer (HAL.lib)... "); 

	kset_color(0x19);
	kgoto_xy(0,2);

	hal_init();

	
	//geninterrupt (0x15);

	/*kprintf("    eqraOS Kernel executed.\n\n");

	kprintf("                ___ ___ ________ _  / __ \\ / __/\n");
	kprintf("               / -_) _ `/ __/ _ `/ / /_/ /_\\ \\  \n");
	kprintf("               \\__/\\_, /_/  \\_,_/  \\____//___/  \n");
	kprintf("                    /_/                         \n");
	*/
	
	//gen_interrupt(0x15);
	return 0;
}
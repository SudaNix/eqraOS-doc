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

#include "kdisplay.h"

void _cdecl main()
{
	int x = 12;

	kclear(31);			// white on blue.

	kgoto_xy(4,4);

	ksetcolor(0x17);

	kprintf("    eqraOS Kernel executed.\n\n");

	kprintf("                ___ ___ ________ _  / __ \\ / __/\n");
	kprintf("               / -_) _ `/ __/ _ `/ / /_/ /_\\ \\  \n");
	kprintf("               \\__/\\_, /_/  \\_,_/  \\____//___/  \n");
	kprintf("                    /_/                         \n");
	

	kprintf("\n\n\nint x = 12 ");
	kprintf("\nx as int............. ");
	kprintf("[%d]",x);
	kprintf("\nx as hex............. ");
	kprintf("[%x]",x);

	kprintf("\n\n               eqraOS v0.1 Copyright (C) 2010 Ahmad Essam");
}
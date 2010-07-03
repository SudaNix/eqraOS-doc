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
#include <string.h>

#include "kdisplay.h"
#include "exception.h"
#include "pmm.h"
#include "mem_info.h"
#include "color.h"
#include <keyboard.h>

void eqraos_screen();
void eqraos_splash();

void exec();
void read_cmd(char* cmd,int n);
bool exec_cmd(char* cmd);
void prompt();
KEY_CODE getch();
void sleep(int ms);


int _cdecl main(multiboot_info* boot_info)
{
	mem_info_init(boot_info);

	hal_init();
	enable_irq();
	execption_init();

	eqraos_splash();
	eqraos_screen();

	keyboard_install(33);

	exec();

	kputs("\neqraOS kernel halt");
	return 0;
}

void eqraos_screen() {

	kclear(GRAY_ON_BLUE);
	kgoto_xy(0,0);

	kset_color(WHITE_ON_CYAN);
	kputs("                                  eqraOS v0.1                                   ");

	kgoto_xy(0,2);

	kset_color(GRAY_ON_BLUE);
}


void exec() {
	char cmd[100] ;
	
	while(1) {
		read_cmd(cmd,98);
		
		bool ret = exec_cmd(cmd);
		
		if (ret)
			break;
	}
}

void prompt() {

	kset_color(LIGHT_RED_ON_BLUE);
	kprintf("\nroot@eqraOS> ");
	kset_color(GRAY_ON_BLUE);
}

void read_cmd(char* cmd,int n) {
	prompt();
	
	KEY_CODE key = KEY_UNKNOWN ;
	bool buffer_char ;
	
	int i=0;
	while (i<n) {
		buffer_char = true;
		key = getch();
		
		if (key == KEY_RETURN)
			break;
		
		if (key == KEY_BACKSPACE) {
			buffer_char = false;
			
			if (i>0) {
				uint32_t x,y;
				kget_xy(&x,&y);
				
				if (x>0)
					kgoto_xy(--x,y);
				else {
					--y;
					x = kget_horz();
				}
				
				// erase the character from display
				kputc(' ');
				kgoto_xy(x,y);
				--i;
			}
		}
		
		
		if (buffer_char) {
			char c = keyboard_key_to_ascii (key);
			if (c) {
				kputc(c);
				cmd[i] = c;
				++i;
			}
		}
		
		sleep(10);
	}
	
	cmd[i] = '\0';
}

KEY_CODE getch() {
	KEY_CODE key = KEY_UNKNOWN;
	
	while (key == KEY_UNKNOWN)
		key = keyboard_get_last_key();
		
	keyboard_discard_last_key();
	return key;
}

void sleep(int ms){
	static int ticks = ms + get_tick_count ();
	while (ticks > get_tick_count ());
}


bool exec_cmd(char* cmd) {
	if (strcmp (cmd, "dump") == 0) {
		mem_info_dump_size();
		mem_info_dump_region();
	}

	else if (strcmp (cmd, "exit") == 0) {
		return true;
	}

	else if (strcmp (cmd, "reboot") == 0) 
		keyboard_reset_system();


	else if (strcmp (cmd, "clear") == 0) {
		eqraos_screen();
	}

	else if (strcmp (cmd, "todo") == 0) {
		kputs("\ncommands can be helpful :\n");
		kputs(" - recov:   Recovery and restore deleted files.\n");
		kputs(" - fixbads: Detect and fix bad sectors.\n");
		kputs(" - fixboot: Fix MBR.\n");
	}

	else if (strcmp (cmd, "eqraos") == 0) {
		kset_color(WHITE_ON_BLUE);
		kputs("\neqraOS v0.1");
		kset_color(GRAY_ON_BLUE);
	}

	else if (strcmp (cmd, "help") == 0) {
		kset_color(WHITE_ON_BLUE);
		kputs("\neqraOS v0.1 copyright (c) 2010 Ahmad Essam.");
		kset_color(GRAY_ON_BLUE);

		kputs("\nUniversity of Khartoum - Faculty of Mathematical Sciences\n\n");
		kputs("Supported commands:\n");
		kputs(" - dump:   Dump memory regions and memory size\n");
		kputs(" - clear:  Clears the display\n");
		kputs(" - reboot: Reboot the computer\n");
		kputs(" - help:   Displays this message\n");
		kputs(" - todo:   Todo task\n");
		kputs(" - exit:   Quits and halts the system\n");
	}

	// invalid command
	else {
		kputs("\nInvalid or not supported command");
	}

	return false;
}

void eqraos_splash() {
	kgoto_xy(0,0);
	kclear(WHITE_ON_BLUE);
	kset_color(WHITE_ON_BLUE);
	kgoto_xy(0,4);

	kputs("          eqraOS v0.1 copyright (c) 2010 Ahmad Essam \n");
	kputs("	University of Khartoum - Faculty of Mathematical Sceinces \n\n\n");
	kprintf("                ___ ___ ________ _  / __ \\ / __/\n");
	kprintf("               / -_) _ `/ __/ _ `/ / /_/ /_\\ \\  \n");
	kprintf("               \\__/\\_, /_/  \\_,_/  \\____//___/  \n");
	kprintf("                    /_/                         \n");

	sleep(1000);
}

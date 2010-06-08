%ifndef MULTIBOOT_INFO
%define MULTIBOOT_INFO

struc multiboot_info
	.flags				resd		1
	.mem_low			resd		1
	.mem_high			resd		1
	.boot_device		resd		1
	.cmd_line			resd		1
	.mods_count			resd		1
	.mods_addr			resd		1
	.sym0				resd		1
	.sym1				resd		1
	.sym2				resd		1
	.mmap_length		resd		1
	.mmap_addr			resd		1
	.drives_length		resd		1
	.drives_addr		resd		1
	.config_table		resd		1
	.bootloader_name	resd		1
	.apm_table			resd		1
	.vbe_control_info	resd		1
	.vbe_mode_info		resd		1
	.vbe_mode			resw		1
	.vbe_interface_seg	resw		1
	.vbe_interface_off	resw		1
	.vbe_interface_len	resw		1
	
endstruc	

%endif ; MULTIBOOT_INFO
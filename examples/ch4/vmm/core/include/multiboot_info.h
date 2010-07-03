#ifndef MULTIBOOT_INFO_H
#define MULTIBOOT_INFO_H

#include <stdint.h>

struct multiboot_info {
	uint32_t	m_flags;
	//uint32_t	m_mem_low;
	//uint32_t	m_mem_high;
	uint64_t	m_mem_size;
	uint32_t	m_boot_device;
	uint32_t	m_cmd_line;
	uint32_t	m_mods_count;
	uint32_t	m_mods_addr;
	uint32_t	m_sym0;
	uint32_t	m_sym1;
	uint32_t	m_sym2;
	uint32_t	m_mmap_length;
	uint32_t	m_mmap_addr;
	uint32_t	m_drives_length;
	uint32_t	m_drives_addr;
	uint32_t	m_config_table;
	uint32_t	m_bootloader_name;
	uint32_t	m_apm_table;
	uint32_t	m_vbe_control_info;
	uint32_t	m_vbe_mode_info;
	uint16_t	m_vbe_mode;
	uint32_t	m_vbe_interface_addr;
	uint16_t	m_vbe_interface_len;
};

#endif // MULTIBOOT_INFO_H
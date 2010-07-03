#ifndef MEM_INFO_H
#define MEM_INFO_H

#include <multiboot_info.h>
#include "kdisplay.h"
#include "pmm.h"
#include "color.h"


extern void mem_info_init(multiboot_info* info);
extern void mem_info_init_region();
extern void mem_info_dump_region();
extern void mem_info_dump_size();
extern void mem_info_blocks_stat();

#endif // MEM_INFO_H
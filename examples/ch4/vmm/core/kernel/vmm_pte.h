#ifndef VMM_PTE_H
#define VMM_PTE_H

#include <stdint.h>

enum PTE_FLAGS {
	I386_PTE_PRESENT 		= 	0x1,
	I386_PTE_WRITABLE 		= 	0x2,
	I386_PTE_USER			=	0x4,
	I386_PTE_WRITETHOUGH	= 	0x8,
	I386_PTE_NOT_CACHEABLE	= 	0x10,
	I386_PTE_ACCESSED		= 	0x20,
	I386_PTE_DIRTY			= 	0x40,
	I386_PTE_PAT			=	0x80,
	I386_PTE_CPU_GLOBAL		=	0x100,
	I386_PTE_LV4_GLOBAL		=	0x200,
	I386_PTE_FRAME			= 	0x7FFFF000
};


extern void pte_add_attrib(uint32_t* e,uint32_t attrib);
extern void pte_del_attrib(uint32_t* e,uint32_t attrib);
extern void pte_set_frame(uint32_t* e,uint32_t addr);
extern uint32_t pte_get_frame(uint32_t e);
extern bool pte_is_present(uint32_t e);
extern bool pte_is_writable(uint32_t e);


#endif // VMM_PTE_H
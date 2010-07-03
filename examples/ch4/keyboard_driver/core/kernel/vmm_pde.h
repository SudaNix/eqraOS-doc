#ifndef VMM_PDE_H
#define VMM_PDE_H

#include <stdint.h>

enum PDE_FLAGS {
	I386_PDE_PRESENT		=	0x1,
	I386_PDE_WRITABLE		=	0x2,
	I386_PDE_USER			=	0x4,
	I386_PDE_PWT			=	0x8,
	I386_PDE_PCD			=	0x10,
	I386_PDE_ACCESSED		=	0x20,
	I386_PDE_DIRTY			=	0x40,
	I386_PDE_4MB			=	0x80,
	I386_PDE_CPU_GLOBAL		=	0x100,
	I386_PDE_LV4_GLOBAL		=	0x200,
	I386_PDE_FRAME			=	0x7FFFF000
};

extern void pde_add_attrib(uint32_t* e,uint32_t attrib);
extern void pde_del_attrib(uint32_t* e,uint32_t attrib);
extern void pde_set_frame(uint32_t* e,uint32_t addr);
extern uint32_t pde_get_frame(uint32_t e);
extern bool pde_is_present(uint32_t e);
extern bool pde_is_writable(uint32_t e);
extern bool pde_is_user(uint32_t e);
extern bool pde_is_4mb(uint32_t e);
extern void pde_enable_global(uint32_t e);

#endif // VMM_PDE_H
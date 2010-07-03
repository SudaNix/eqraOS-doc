#include "vmm_pde.h"



void pde_add_attrib(uint32_t* e,uint32_t attrib) {
	*e = *e | attrib; 
}

void pde_del_attrib(uint32_t* e,uint32_t attrib) {
	*e = *e & ~attrib;
}

void pde_set_frame(uint32_t* e,uint32_t addr) {
	*e = ( *e & ~I386_PDE_FRAME ) | addr ;
}

uint32_t pde_get_frame(uint32_t e) {
	return e & I386_PDE_FRAME ;
}

bool pde_is_present(uint32_t e) {
	return e & I386_PDE_PRESENT;
}

bool pde_is_writable(uint32_t e) {
	return e & I386_PDE_WRITABLE ;
}

bool pde_is_user(uint32_t e) {
	return e & I386_PDE_USER;
}

bool pde_is_4mb(uint32_t e) {
	return e & I386_PDE_4MB;
}

void pde_enable_global(uint32_t e) {

}
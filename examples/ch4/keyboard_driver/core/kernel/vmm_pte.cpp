#include "vmm_pte.h"

void pte_add_attrib(uint32_t* e,uint32_t attrib) {
	*e = *e | attrib; 
}

void pte_del_attrib(uint32_t* e,uint32_t attrib) {
	*e = *e & ~attrib;
}

void pte_set_frame(uint32_t* e,uint32_t addr) {
	*e = ( *e & ~I386_PTE_FRAME ) | addr ;
}

uint32_t pte_get_frame(uint32_t e) {
	return e & I386_PTE_FRAME ;
}

bool pte_is_present(uint32_t e) {
	return e & I386_PTE_PRESENT;
}

bool pte_is_writable(uint32_t e) {
	return e & I386_PTE_WRITABLE ;
}
#include "vmm.h"

#include <string.h>
#include "vmm.h"
#include "pmm.h"

#define PAGE_SIZE 				4096
#define PT_ADDRESS_SPACE_SIZE	0x400000
#define PD_ADDRESS_SPACE_SIZE	0x100000000

page_dir_table* _cur_page_dir = 0;
uint32_t	_cur_page_dir_base_register = 0;

void vmm_init() {
	page_table* pt = (page_table*) pmm_alloc();
	if (!pt)
		return ;
	
	vmm_page_table_clear(pt);
	
	for (int i=0, frame =0; i < 1024 ; ++i, frame += 4096) {
		uint32_t page = 0;
		pte_add_attrib(&page,I386_PTE_PRESENT);
		pte_set_frame(&page,frame);
		
		pt->pte[vmm_vir_to_page_table_index(frame)] = page;
	}
	
	page_dir_table* pd = (page_dir_table*) pmm_allocs(3);
	if (!pd)
		return ;
		
	vmm_page_dir_table_clear(pd);
	
	uint32_t* pde = vmm_page_dir_table_lookup_entry(pd,0);
	pde_add_attrib(pde,I386_PDE_PRESENT);
	pde_add_attrib(pde,I386_PDE_WRITABLE);
	pde_set_frame(pde,(uint32_t)pt);
	
	_cur_page_dir_base_register = (uint32_t) &pd->pde;
	vmm_switch_page_dir_table(pd);
	
	pmm_enable_paging(true);
	
}

bool vmm_alloc_page(uint32_t* e) {
	void* p = pmm_alloc();
	if (!p)
		return false;
	
	pte_set_frame(e,(uint32_t)p);
	pte_add_attrib(e,I386_PTE_PRESENT);
	
	return true;
}

void vmm_free_page(uint32_t* e) {
	void* p = (void*)pte_get_frame(*e);
	
	if (p)
		pmm_dealloc(p);
		
	pte_del_attrib(e,I386_PTE_PRESENT);
	
}


bool vmm_switch_page_dir_table(page_dir_table* pd) {
	if (!pd)
		return false;
	
	_cur_page_dir = pd;
	pmm_load_PDBR(_cur_page_dir_base_register);
	return true;
}

page_dir_table* vmm_get_page_dir_table() {
	return _cur_page_dir;
}

void vmm_flush_tlb_entry(uint32_t addr) {
#ifdef _MSC_VER
	_asm {
		cli
		invlpg addr
		sti
	}
#endif
}

void vmm_page_table_clear(page_table* pt) {
	if (pt)
		memset(pt,0,sizeof(page_table));
}

uint32_t vmm_vir_to_page_table_index(uint32_t addr) {
	if ( addr  >= PT_ADDRESS_SPACE_SIZE )
		return 0;
	else
		return addr / PAGE_SIZE;
}

uint32_t* vmm_page_table_lookup_entry(page_table* pt,uint32_t addr) {
	if (pt)
		return &pt->pte[vmm_vir_to_page_table_index(addr)];
	else
		return 0;
}

uint32_t vmm_vir_to_page_dir_table_index(uint32_t addr) {
	if ( addr >= PD_ADDRESS_SPACE_SIZE )
		return 0;
	else
		return addr / PAGE_SIZE;
}

void vmm_page_dir_table_clear(page_dir_table* pd) {
	if (pd)
		memset(pd,0,sizeof(page_dir_table));
}

uint32_t* vmm_page_dir_table_lookup_entry(page_dir_table* pd,uint32_t addr) {
	if (pd)
		return &pd->pde[vmm_vir_to_page_table_index(addr)];
	else 
		return 0;
}

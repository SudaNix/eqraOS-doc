#ifndef VMM_H
#define VMM_H

#include <stdint.h>

#include "vmm_pte.h"
#include "vmm_pde.h"

#define PTE_N 1024
#define PDE_N 1024

struct page_table {
	uint32_t pte[PTE_N];
};

struct page_dir_table {
	uint32_t pde[PDE_N];
};

extern void vmm_init();
extern bool vmm_alloc_page(uint32_t* e); 
extern void vmm_free_page(uint32_t* e);
extern bool vmm_switch_page_dir_table(page_dir_table*);
extern page_dir_table* vmm_get_page_dir_table();
extern void vmm_flush_tlb_entry(uint32_t addr);
extern void vmm_page_table_clear(page_table* pt);
extern uint32_t vmm_vir_to_page_table_index(uint32_t addr);
extern uint32_t* vmm_page_table_lookup_entry(page_table* pt,uint32_t addr);
extern uint32_t vmm_vir_to_page_dir_table_index(uint32_t addr);
extern void vmm_page_dir_table_clear(page_dir_table* pd);
extern uint32_t* vmm_page_dir_table_lookup_entry(page_dir_table* pd,uint32_t addr);


#endif // VMM_H
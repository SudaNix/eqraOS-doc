#ifndef PMM_H
#define PMM_H

#include <stdint.h>

extern void pmm_init(size_t size,uint32_t base);
extern void pmm_init_region(uint32_t base,size_t size);
extern void pmm_deinit_region(uint32_t base,size_t size);
extern void* pmm_alloc();
extern void* pmm_allocs(size_t size);
extern void pmm_dealloc(void*);
extern void pmm_deallocs(void*,size_t);
extern size_t pmm_get_mem_size();
extern uint32_t pmm_get_block_count();
extern uint32_t pmm_get_used_block_count();
extern uint32_t pmm_get_free_block_count();
extern uint32_t pmm_get_block_size();
extern void pmm_enable_paging(bool);
extern bool pmm_is_paging();
extern void pmm_load_PDBR(uint32_t);
extern uint32_t pmm_get_PDBR();

#endif // PMM_H
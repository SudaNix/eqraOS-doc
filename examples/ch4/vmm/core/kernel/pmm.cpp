#include <string.h>
#include "pmm.h"
#include "kdisplay.h"

#define PMM_BLOCK_PER_BYTE	8
#define PMM_PAGE_SIZE		4096
#define PMM_BLOCK_SIZE		PMM_PAGE_SIZE
#define PMM_BLOCK_ALIGN		PMM_BLOCK_SIZE

static uint32_t _pmm_mem_size = 0;
static uint32_t _pmm_max_blocks = 0;
static uint32_t _pmm_used_blocks = 0;
static uint32_t* _pmm_mmap = 0;

inline void mmap_set(int bit) {
	_pmm_mmap[bit/32] = _pmm_mmap[bit/32] | (1 << (bit%32)) ;
}

inline void mmap_unset(int bit) {
	_pmm_mmap[bit/32] = _pmm_mmap[bit/32] & ~(1 << (bit%32));
}

inline bool mmap_test(int bit) {
	return _pmm_mmap[bit/32] & (1 << (bit%32));
}

int mmap_find_first() {
	
	for (uint32_t i=0;i<pmm_get_free_block_count()/32;++i) {
		if (_pmm_mmap[i] != 0xffffffff) {
			for (int j=0;j<32;++j) {
				int bit = 1 << j;
				if (!(_pmm_mmap[i] & bit))
					return i*32+j;
			}
		}
	}
	
	return -1;
}

int mmap_find_squence(size_t s) {
	if (s == 0)
		return -1;
	
	if (s == 1)
		return mmap_find_first();
		
	for (uint32_t i=0;i<pmm_get_free_block_count()/32;++i) {
		if (_pmm_mmap[i] != 0xffffffff) {
			for (int j=0;j<32;++j) {
				int bit = 1 << j;
				if (!(_pmm_mmap[i] & bit)) {
					
					int start_bit = i*32 + bit;
					uint32_t free_bit = 0;
					
					for (uint32_t count=0;count<=s;++count) {
						if (!(mmap_test(start_bit+count)))
							free_bit++;
							
						if (free_bit == s)
							return i*32+j;
					}
				}
			}
		}
	}
	
	return -1;
}


void pmm_init(size_t size,uint32_t mmap_base) {
	_pmm_mem_size = size;
	_pmm_mmap = (uint32_t*)mmap_base;
	_pmm_max_blocks = _pmm_mem_size*1024 / PMM_BLOCK_SIZE;
	_pmm_used_blocks = _pmm_max_blocks;  // all memory used by default
	
	memset(_pmm_mmap,0xf,_pmm_max_blocks/PMM_BLOCK_PER_BYTE);
}

void pmm_init_region(uint32_t base,size_t size) {
	int align = base/PMM_BLOCK_SIZE;
	int blocks = size/PMM_BLOCK_SIZE;
	
	for (;blocks >=0;blocks--) {
		mmap_unset(align++);
		_pmm_used_blocks--;
	}
	
	mmap_set(0);
}

void pmm_deinit_region(uint32_t base,size_t size) {
	int align = base/PMM_BLOCK_SIZE;
	int blocks = size/PMM_BLOCK_SIZE;
	
	for (;blocks >=0;blocks--) {
		mmap_set(align++);
		_pmm_used_blocks++;
	}
}

void* pmm_alloc() {
	if (pmm_get_free_block_count() <= 0)
		return 0;
		
	int block = mmap_find_first();
	
	if (block == -1)
		return 0;
		
	mmap_set(block);
	
	uint32_t addr = block * PMM_BLOCK_SIZE;
	_pmm_used_blocks++;
	
	return (void*) addr;
}

void* pmm_allocs(size_t s) {
	if (pmm_get_free_block_count() <= s)
		return 0;
		
	int block = mmap_find_squence(s);
	
	if (block == -1)
		return 0;	
		
	for (uint32_t i=0;i<s;++i) 
		mmap_set(block+i);
	
	uint32_t addr = block * PMM_BLOCK_SIZE;
	_pmm_used_blocks += s;
	
	return (void*) addr;
}

void pmm_dealloc(void* p) {
	uint32_t addr = (uint32_t)p;
	int block = addr / PMM_BLOCK_SIZE;
	mmap_unset(block);
	_pmm_used_blocks--;
}

void pmm_deallocs(void* p,size_t s) {
	uint32_t addr = (uint32_t)p;
	int block = addr / PMM_BLOCK_SIZE;
	
	for (uint32_t i=0;i<s;++i) 
		mmap_unset(block+i);
		
	_pmm_used_blocks -= s;	
}

size_t pmm_get_mem_size() {
	return _pmm_mem_size;
}

uint32_t pmm_get_block_count() {
	return _pmm_max_blocks;
}

uint32_t pmm_get_used_block_count() {
	return _pmm_used_blocks;
}

uint32_t pmm_get_free_block_count() {
	return _pmm_max_blocks - _pmm_used_blocks;
}

uint32_t pmm_get_block_size() {
	return PMM_BLOCK_SIZE;
}

void pmm_enable_paging(bool val) {
#ifdef _MSC_VER
	_asm {
	
		mov eax,cr0
		cmp [val],1
		je enable
		jmp disable
	
	enable:
		or eax,0x80000000  // set last bit
		mov cr0,eax
		jmp done
		
	disable:
		and eax,0x7fffffff // unset last bit
		mov cr0,eax
	
	done:
	}
	
#endif
}

bool pmm_is_paging() {
	uint32_t val = 0;
#ifdef _MSC_VER
	_asm {
		mov eax,cr0
		mov [val],eax
	}
	
	if ( val & 0x80000000 )
		false;
	else
		true;
#endif
}

void pmm_load_PDBR(uint32_t addr) {
#ifdef _MSC_VER
	_asm {
		mov eax,[addr]
		mov cr3,eax
	}
#endif
}

uint32_t pmm_get_PDBR() {
#ifdef _MSC_VER
	_asm {
		mov eax,cr3
		ret
	}
#endif
}

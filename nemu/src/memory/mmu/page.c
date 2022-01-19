#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
typedef union {
	struct
	{
		uint32_t pf_off : 12;
		uint32_t pt_idx : 10;
		uint32_t pdir_idx : 10;
	};
	uint32_t addr;
} PageAddr;

paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
#ifdef IA32_PAGE
	PageAddr *addr = (void *)&laddr;
	PDE* pde = (PDE*)(hw_mem + ((cpu.cr3.pdbr << 12) + 4 * addr->pdir_idx));
	assert(pde->present == 1);
	PTE* pte = (PTE*)(hw_mem + ((pde->page_frame << 12) + 4 * addr->pt_idx));
	assert(pte->present == 1);
	return (pte->page_frame << 12) | (laddr & PAGE_MASK);
#else
    return 0;
#endif
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}

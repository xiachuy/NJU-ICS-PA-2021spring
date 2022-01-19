#include "nemu.h"
#include "cpu/cpu.h"
#include "memory/memory.h"
#include "device/mm_io.h"
#include <memory.h>
#include <stdio.h>

uint8_t hw_mem[MEM_SIZE_B];

uint32_t hw_mem_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
	memcpy(&ret, hw_mem + paddr, len);
	return ret;
}

void hw_mem_write(paddr_t paddr, size_t len, uint32_t data)
{
	memcpy(hw_mem + paddr, &data, len);
}

uint32_t paddr_read(paddr_t paddr, size_t len)
{
	uint32_t ret = 0;
#ifdef HAS_DEVICE_VGA
    if(is_mmio(paddr) != -1){
        return mmio_read(paddr, len, is_mmio(paddr));
    }
#endif


#ifdef CACHE_ENABLED
		ret = cache_read(paddr, len);     // 通过cache进行读
#else
		ret = hw_mem_read(paddr, len);
#endif
	return ret;
}

void paddr_write(paddr_t paddr, size_t len, uint32_t data)
{

#ifdef HAS_DEVICE_VGA
    if(is_mmio(paddr) != -1){
        mmio_write(paddr, len, data, is_mmio(paddr));
        return;
    }
#endif

#ifdef CACHE_ENABLED
	cache_write(paddr, len, data);    // 通过cache进行写
#else
	hw_mem_write(paddr, len, data);
#endif
}

uint32_t laddr_read(laddr_t laddr, size_t len)
{
    assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_PAGE
	return paddr_read(laddr, len);
#else
    paddr_t paddr = laddr;
    if(cpu.cr0.pg == 1){
        //开启分页模式
        if(((laddr + len - 1) >> 12) != (laddr >> 12)){
            //跨页
            size_t nextPage = (((laddr >> 12) + 1) << 12) - laddr;
            paddr_t p1 = page_translate(laddr);
            paddr_t p2 = page_translate(laddr + nextPage);
            uint32_t res1 = paddr_read(p1, nextPage);
            uint32_t res2 = paddr_read(p2, len - nextPage);
            return (res2 << (8 * nextPage)) | res1 ;
        }else{
            paddr = page_translate(laddr);
        }
    }
    return paddr_read(paddr, len);
#endif

}

void laddr_write(laddr_t laddr, size_t len, uint32_t data)
{
    assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_PAGE
	paddr_write(laddr, len, data);
#else
    paddr_t paddr = laddr;
    if(cpu.cr0.pg == 1){
        if(((laddr + len - 1) >> 12) != (laddr >> 12)){
            //跨页，需要分开两次
            size_t nextPage = (((laddr >> 12) + 1) << 12) - laddr;
            paddr_t p1 = page_translate(laddr);
            paddr_t p2 = page_translate(laddr + nextPage);
            paddr_write(p1, nextPage, (data & ((1 << (8*nextPage))-1)));
            paddr_write(p2, len - nextPage, data >> (8*nextPage));
            return;
        }else{
            paddr = page_translate(laddr);
        }
    }
    paddr_write(paddr, len, data);
#endif
}

uint32_t vaddr_read(vaddr_t vaddr, uint8_t sreg, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_read(vaddr, len);
#else
	uint32_t laddr = vaddr;
	if(cpu.cr0.pe == 1) {
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_read(laddr, len);
#endif
}

void vaddr_write(vaddr_t vaddr, uint8_t sreg, size_t len, uint32_t data)
{
	assert(len == 1 || len == 2 || len == 4);
#ifndef IA32_SEG
	return laddr_write(vaddr, len, data);
#else
	uint32_t laddr = vaddr;
	if(cpu.cr0.pe == 1) {
		laddr = segment_translate(vaddr, sreg);
	}
	return laddr_write(laddr, len, data);
#endif
}

void init_mem()
{
	// clear the memory on initiation
	memset(hw_mem, 0, MEM_SIZE_B);
#ifdef CACHE_ENABLED
	init_cache();                             // 初始化cache
#endif

#ifdef TLB_ENABLED
	make_all_tlb();
	init_all_tlb();
#endif
}

uint32_t instr_fetch(vaddr_t vaddr, size_t len)
{
	assert(len == 1 || len == 2 || len == 4);
	return vaddr_read(vaddr, SREG_CS, len);
}

uint8_t *get_mem_addr()
{
	return hw_mem;
}

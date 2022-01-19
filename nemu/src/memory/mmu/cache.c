#include "memory/mmu/cache.h"
#include <stdlib.h>
#include <time.h>


cache_set cache_mem[LINE_SIZE / SET_SIZE];



uint32_t get_block_offset(paddr_t paddr){
    return (uint32_t)paddr & 0x3F;
}

uint32_t get_tag(paddr_t paddr){
    return ((uint32_t)paddr) >> (7+6);
}

uint32_t get_set_no(paddr_t paddr){
    return (((uint32_t)paddr) >> 6) & 0x7F;
}

uint8_t cache_read_byte(paddr_t paddr){
    uint32_t set_no = get_set_no(paddr);
    uint32_t tag = get_tag(paddr);
    uint32_t off = get_block_offset(paddr);
    uint8_t ret = 0;
    
    
    cache_set set = cache_mem[set_no]; 
    bool hit = false;
    
    for(uint8_t i = 0; i < SET_SIZE; ++i){
        if(set.cache_lines[i].valid_bit == true && set.cache_lines[i].tag == tag){
            hit = true;
            ret = set.cache_lines[i].cache_blocks[off];
            break;
        }
    }
    
    if(!hit){
        // find a line which is invalid
        // if all lines are valid, replace a random line
        uint8_t invalid_line = -1;
        
        for(uint8_t i = 0; i < SET_SIZE; ++i){
            if(set.cache_lines[i].valid_bit == false){
                invalid_line = i;
                break;
            }
        }
        if(invalid_line == -1){
            srand((unsigned int)time(NULL));
            invalid_line = (uint8_t)(((uint32_t)rand()) % SET_SIZE);
        }
        
        set.cache_lines[invalid_line].tag = tag;
        set.cache_lines[invalid_line].valid_bit = true;
        
        /*for(uint32_t i = 0; i < CACHE_BLOCK_SIZE; ++i){
            paddr_t st_paddr = paddr & 0xFFFFFFF0;
            set.cache_lines[invalid_line].cache_blocks[i] = hw_mem_read(st_paddr + i, 1);
        }*/
        memcpy(set.cache_lines[invalid_line].cache_blocks, hw_mem + ((uint32_t)paddr & 0xFFFFFFC0), CACHE_BLOCK_SIZE);
        
        ret = set.cache_lines[invalid_line].cache_blocks[off];
    }
    
    return ret;
}

void cache_write_byte(paddr_t paddr, uint8_t data){
    // no write allocate
    // hw_mem must be wroten! 
    uint32_t set_no = get_set_no(paddr);
    uint32_t tag = get_tag(paddr);
    uint32_t off = get_block_offset(paddr);
    
    cache_set set = cache_mem[set_no]; 
    bool hit = false;
    
    uint8_t i = 0;
    for(; i < SET_SIZE; ++i){
        if(set.cache_lines[i].valid_bit == true && set.cache_lines[i].tag == tag){
            hit = true;
            break;
        }
    }
    if(hit){
        set.cache_lines[i].cache_blocks[off] = data;
    }
    return;
}

// init the cache
void init_cache()
{
	// implement me in PA 3-1
	memset(cache_mem, 0, sizeof(cache_mem));
	
}

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data)
{
	// implement me in PA 3-1
	memcpy(hw_mem + paddr, &data, len);
	//hw_mem_write(paddr, len, data);
	for(uint32_t i = 0; i < len; ++i){
	    uint8_t split_data = (uint8_t)((data >> (8 * i)) & 0xFF);
	    cache_write_byte(paddr + i, split_data);
	}
}

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len)
{
	// implement me in PA 3-1
	// consider 4 bytes cross two
	// read every byte to form the result?
	uint32_t ret = 0;
	for(uint32_t i = 0; i < len; ++i){
	    uint32_t tmp = 0;
	    tmp = cache_read_byte(paddr + i);
	    ret |=   (tmp << (8*i)); 
	}
	return ret;
}



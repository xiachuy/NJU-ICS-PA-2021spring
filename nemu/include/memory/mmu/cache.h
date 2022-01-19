#ifndef __CACHE_H__
#define __CACHE_H__

#include "nemu.h"
#include "memory/memory.h"
#define CACHE_BLOCK_SIZE  64
#define LINE_SIZE  1024
#define SET_SIZE  8

typedef struct cache_line{
    uint8_t cache_blocks[CACHE_BLOCK_SIZE];
    uint32_t tag;
    bool valid_bit;
}cache_line;

typedef struct cache_set{
  cache_line cache_lines[SET_SIZE];
}cache_set;

extern cache_set cache_mem[LINE_SIZE / SET_SIZE];

#ifdef CACHE_ENABLED



// init the cache
void init_cache();

// write data to cache
void cache_write(paddr_t paddr, size_t len, uint32_t data);

// read data from cache
uint32_t cache_read(paddr_t paddr, size_t len);

#endif

#endif

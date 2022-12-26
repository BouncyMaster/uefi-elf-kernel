#ifndef MEMORY_H
#define MEMORY_H

#include <int.h>
#include "boot.h"

extern u64 freeMemory;
extern u64 usedMemory;
extern u64 reservedMemory;

void memory_lock_page(void *address);
void memory_lock_pages(void *address, u64 pageCount);
void memory_free_page(void *address);
void memory_free_pages(void *address, u64 pageCount);
void memory_reserve_page(void *address);
void memory_reserve_pages(void *address, u64 pageCount);
void *memory_request_page(void);

u64 memory_init(Memory_Descriptor *mMap, u64 mMapSize, u64 mMapDescSize);

#endif // MEMORY_H

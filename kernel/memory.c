/*
 * 1 page = 4 KB = 4096 B
 * EFI Returns memory descriptor size in pages, not bytes.
 * Memory management using a bitmap: 1 bit = 1 page (so a u8 maps 8 pages).
 * Maps even non-usable memory.
 */

#include <int.h>
#include "boot.h"
#include "bitmap.h"

/*
 * TODO: This is poorly optimized, lots of divisions, maybe we can pass
 * pageIndex to functions instead of addresses.
 */

static u8 *bitmap;
static u64 bitmapSize;

u64 freeMemory;
u64 usedMemory = 0;
u64 reservedMemory = 0;

/*
 * TODO: maybe move this to a new file for handling buffers
 * TODO2: if we are guaranteed that the buffer size is divisible by 8 bytes,
 * then maybe we can cast to u64 instead for performance;
 * Not 100% sure it will be faster.
 */
// Zero out the buffer
static void
buffer_zero(void *buffer, u64 size)
{
	for (u64 i = 0; i < size; i++){
		*(u8 *)(buffer + i) = 0;
	}
}

void
memory_lock_page(void *address)
{
	// TODO: check if physicalAddress is divisible by the size of 1 page
	u64 index = (u64)address / 4096;

	// TODO: Give error if this is true
	// Already reserved or locked
	if (bitmap_get(bitmap, index))
		return;

	bitmap_set(bitmap, index, 1);
	freeMemory -= 4096;
	usedMemory += 4096;
}

void
memory_lock_pages(void *address, u64 pageCount)
{
	for (u64 i = 0; i < pageCount; i++)
		memory_lock_page((void *)((u64)address + i));
}

void
memory_free_page(void *address)
{
	u64 index = (u64)address / 4096;

	// TODO: Give error if this is true
	// Already free
	if (!bitmap_get(bitmap, index))
		return;

	bitmap_set(bitmap, index, 0);
	freeMemory += 4096;
	usedMemory -= 4096;
}

void
memory_free_pages(void *address, u64 pageCount)
{
	for (u64 i = 0; i < pageCount; i++)
		memory_free_page((void *)((u64)address + i));
}

void
memory_reserve_page(void *address)
{
	u64 index = (u64)address / 4096;

	// TODO: Give error if this is true
	// Already reserved or locked
	if (bitmap_get(bitmap, index))
		return;

	bitmap_set(bitmap, index, 1);
	freeMemory -= 4096;
	reservedMemory += 4096;
}

void
memory_reserve_pages(void *address, u64 pageCount)
{
	for (u64 i = 0; i < pageCount; i++)
		memory_reserve_page((void *)((u64)address + i));
}

// Request a single free page
void *
memory_request_page(void)
{
	void *memory;
	// Iterate over all the pages
	for (u64 i = 0; i < bitmapSize * 8; i++){
		if (bitmap_get(bitmap, i))
			continue;

		memory = (void *)(i * 4096);
		memory_lock_page(memory);
		return memory;
	}
	return 0; // OOM
}

void
memory_init(Memory_Descriptor *mMap, u64 mMapSize, u64 mMapDescSize)
{
	Memory_Descriptor *desc;
	u64 totalPages = 0;
	void *largestFreeMemSeg;

	// NOTE: size is stored as number of pages
	u64 largestFreeMemSegSize = 0;

	/*
	 * Find the largest memory segment of type EfiConventionalMemory and 
	 * put our bitmap there.
	 * Also, calculate the total number of pages for the bitmap length.
	 */
	for (u64 i = 0; i < mMapSize; i += mMapDescSize){
		// TODO: see boot.h
		desc = (Memory_Descriptor *)((u64)mMap + i);
		totalPages += desc->pageCount;

		if (desc->type == 7 && desc->pageCount > largestFreeMemSegSize){
			largestFreeMemSeg = desc->physicalStart;
			largestFreeMemSegSize = desc->pageCount;
		}
	}

	bitmap = largestFreeMemSeg;
	// TODO: sanity check bitmapSize <= largestFreeMemSegSize
	bitmapSize = totalPages / 8 + 1; // Size is in bytes for this
	freeMemory = totalPages * 4096;

	buffer_zero(bitmap, bitmapSize);
	// Specify which pages are used for the bitmap.
	// Worst case we overallocate by 1 page = 4KB
	memory_lock_pages(bitmap, bitmapSize / 4096 + 1);

	/*
	 * Find all memory that is not EfiConventionalMemory and map it as
	 * reserved.
	 */
	for (u64 i = 0; i < mMapSize; i += mMapDescSize){
		desc = (Memory_Descriptor *)((u64)mMap + i);
		if (desc->type != 7)
			memory_reserve_pages(desc->physicalStart,
				desc->pageCount);
	}
}

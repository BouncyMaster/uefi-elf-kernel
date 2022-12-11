// Contains definitions for boot structures.

#ifndef KERNEL_H
#define KERNEL_H

#include <int.h>

/*
 * @brief Memory region descriptor.
 * Describes a region of memory. This is passed to the kernel by the bootloader.
 */
typedef struct {
	u32 type;
	u64 physical_start;
	u64 virtual_start;
	u64 count;
	u64 attributes;
} Memory_Map_Descriptor;

typedef struct {
	u32 *framebuffer;
	u32  xRes;
	u32  yRes;
} Video_Mode_Info;

/*
 * @brief Boot info struct.
 * Contains information passed to the kernel at boot time.
 * This should be kept separate from the kernel implementation to improve the
 * portability of the implementation:
 */
typedef struct {
	Memory_Map_Descriptor	*memoryMap;
	u64			 memoryMapSize;
	u64			 memoryMapDescriptorSize;
	Video_Mode_Info		 video;
} Boot_Info;

#endif // KERNEL_H

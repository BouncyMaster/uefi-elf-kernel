// Contains definitions for boot structures.

#ifndef BOOT_H
#define BOOT_H

#include <int.h>

/*
 * @brief Memory region descriptor.
 * Describes a region of memory. This is passed to the kernel by the bootloader.
 */
typedef struct {
	u32 type;
	void *physicalStart;
	void *virtualStart;
	u64 pageCount;
	u64 attribute;
} Memory_Descriptor;

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
	Memory_Descriptor	*memoryMap;
	u64			 memoryMapSize; // Total size
	u64			 memoryMapDescriptorSize; // Size of 1 element
	Video_Mode_Info		 video;
} Boot_Info;

#endif // BOOT_H

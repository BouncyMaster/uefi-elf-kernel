// Contains definitions for boot structures.

#ifndef KERNEL_H
#define KERNEL_H

#include <int.h>

/*
 * @brief Memory region descriptor.
 * Describes a region of memory. This is passed to the kernel by the bootloader.
 */
typedef struct {
	uint32 type;
	uint64 physical_start;
	uint64 virtual_start;
	uint64 count;
	uint64 attributes;
} Memory_Map_Descriptor;

typedef struct {
	void	*framebufferPointer;
	uint32	 horizontalRes;
	uint32	 verticalRes;
	uint32	 pixelsPerScanline;
} Video_Mode_Info;

/*
 * @brief Boot info struct.
 * Contains information passed to the kernel at boot time.
 * This should be kept separate from the kernel implementation to improve the
 * portability of the implementation:
 */
typedef struct {
	Memory_Map_Descriptor	*memoryMap;
	uint64			 memoryMapSize;
	uint64			 memoryMapDescriptorSize;
	Video_Mode_Info		 videoModeInfo;
} Boot_Info;

#endif // KERNEL_H
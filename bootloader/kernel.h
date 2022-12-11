#ifndef KERNEL_H
#define KERNEL_H

typedef struct {
	EFI_PHYSICAL_ADDRESS	framebuffer;
	UINT32			xRes;
	UINT32			yRes;
} Video_Mode_Info;

/*
 * @brief Boot info struct.
 * Contains information passed to the kernel at boot time.
 * This should be kept separate from the kernel implementation to improve the
 * portability of the implementation:
 */
typedef struct {
	EFI_MEMORY_DESCRIPTOR	*memoryMap;
	UINTN			 memoryMapSize;
	UINTN			 memoryMapDescriptorSize;
	Video_Mode_Info		 video;
} Boot_Info;

#endif // KERNEL_H

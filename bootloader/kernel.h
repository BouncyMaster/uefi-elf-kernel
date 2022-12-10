#ifndef KERNEL_H
#define KERNEL_H

typedef struct {
	VOID	*framebufferPointer;
	UINT32	 horizontalRes;
	UINT32	 verticalRes;
	UINT32	 pixelsPerScanline;
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
	Video_Mode_Info		 videoModeInfo;
} Boot_Info;

#endif // KERNEL_H

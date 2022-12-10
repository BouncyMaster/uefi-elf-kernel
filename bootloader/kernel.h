#ifndef KERNEL_H
#define KERNEL_H

typedef struct {
	VOID	*framebufferPointer;
	UINT32	 horizontalRes;
	UINT32	 verticalRes;
	UINT32	 pixelsPerScanline;
} Kernel_Video_Mode_Info;

/*
 * @brief Kernel info struct.
 * Contains information passed to the kernel at boot time.
 * This should be kept separate from the kernel implementation to improve the
 * portability of the implementation:
 * This definition uses the `EFI_MEMORY_DESCRIPTOR` and `UINTN` types so that it
 * will automatically use the correct types for the target architecture. The
 * corresponding definition within the kernel should have more architecture
 * specific types.
 */
typedef struct {
	EFI_MEMORY_DESCRIPTOR	*memoryMap;
	UINTN			 memoryMapSize;
	UINTN			 memoryMapDescriptorSize;
	Kernel_Video_Mode_Info	 videoModeInfo;
} Kernel_Info;

#endif // KERNEL_H

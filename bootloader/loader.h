// Contains functionality for loading the Kernel ELF executable.

#ifndef LOADER_H
#define LOADER_H

/*
 * @brief Loads the kernel binary image into memory.
 * This will load the kernel binary image and validate it. If the kernel binary
 * is valid its executable program segments are loaded into memory.
 * @param[in] root	The root file system FILE entity to load the kernel
 *			binary from.
 * @param[in] filename	The kernel filename on the boot partition.
 * @return address of entry point of the program.
 */
EFI_PHYSICAL_ADDRESS load_kernel(EFI_FILE_PROTOCOL * const root,
	CHAR16 * const filename);

#endif // LOADER_H

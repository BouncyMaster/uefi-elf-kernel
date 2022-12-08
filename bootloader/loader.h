// Contains functionality for loading the Kernel ELF executable.

#ifndef LOADER_H
#define LOADER_H

/*
 * @brief Loads an ELF segment.
 * Loads an ELF program segment into memory.
 * This will read the ELF segment from the kernel binary, allocate the pages
 * necessary to load the segment into memory and then copy the segment to its
 * required physical memory address.
 * @param[in] kernel_img_file The Kernel EFI file entity to read from.
 * @param[in] segment_file_offset The segment's offset into the ELF binary.
 * @param[in] segment_file_size The segment's size in the ELF binary.
 * @param[in] segment_memory_size The size of the segment loaded into memory.
 * @param[in] segment_physical_address The physical memory address to load the segment to.
 */
void load_segment(IN EFI_FILE* const kernel_img_file,
	IN EFI_PHYSICAL_ADDRESS const segment_file_offset,
	IN UINTN const segment_file_size,
	IN UINTN const segment_memory_size,
	IN EFI_PHYSICAL_ADDRESS const segment_physical_address);

/*
 * @brief Loads the ELF program segments.
 * Loads the Kernel ELF binary's program segments into memory.
 * @param[in] kernel_img_file The Kernel EFI file entity to read from.
 * @param[in] file_class The ELF file class, whether the program is 32 or 64bit.
 * @param[in] kernel_header_buffer The Kernel header buffer.
 * @param[in] kernel_program_headers_buffer The kernel program headers buffer.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
void load_program_segments(IN EFI_FILE* const kernel_img_file,
	IN Elf_File_Class const file_class,
	IN VOID* const kernel_header_buffer,
	IN VOID* const kernel_program_headers_buffer);

/*
 * @brief Loads the Kernel binary image into memory.
 * This will load the Kernel binary image and validates it. If the kernel binary
 * is valid its executable program segments are loaded into memory.
 * @param[in]   root_file_system The root file system FILE entity to load the
 *              kernel binary from.
 * @param[in]   kernel_image_filename The kernel filename on the boot partition.
 * @param[out]  kernel_entry_point The entry point memory address for
 *              the kernel.
 * @return The program status.
 * @retval EFI_SUCCESS    If the function executed successfully.
 * @retval other          Any other value is an EFI error code.
 */
void load_kernel(EFI_FILE_PROTOCOL * const root, CHAR16 * const filename,
	EFI_PHYSICAL_ADDRESS *entryPoint);

#endif // LOADER_H

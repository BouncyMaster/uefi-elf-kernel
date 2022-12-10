#include "efilib.h"
#include "elf.h"

/*
 * @brief Loads an ELF segment.
 * Loads an ELF program segment into memory.
 * This will read the ELF segment from the kernel binary, allocate the pages
 * necessary to load the segment into memory and then copy the segment to its
 * required physical memory address.
 * @param[in] file		The Kernel EFI file entity to read from.
 * @param[in] offset		The segment's offset into the ELF binary.
 * @param[in] fileSize		The segment's size in the ELF binary.
 * @param[in] memorySize	The size of the segment loaded into memory.
 * @param[in] physicalAddress	The physical memory address to load the segment to.
 */
static void
load_segment(EFI_FILE_PROTOCOL * const file, const UINT64 offset,
	UINTN fileSize, const UINTN memorySize,
	EFI_PHYSICAL_ADDRESS physicalAddress)
{
	EFI_STATUS status;
	// Buffer to hold the segment data
	void *programData;
	// The number of pages to allocate
	UINTN pageCount = EFI_SIZE_TO_PAGES((UINTN)memorySize);

	status = file->SetPosition(file, offset);
	efi_assert(status, L"load:segment:SetPosition");

	status = BOOT_SERVICES->AllocatePages(AllocateAddress, EfiLoaderData,
		pageCount, &physicalAddress);
	efi_assert(status, L"load:segment:AllocatePages");

	/*
#ifdef DEBUG
	// TODO: is the check needed?
	if (!fileSize)
		err_handle(EFI_LOAD_ERROR, L"load:segment:fileSize");
#endif
	*/
	if (fileSize > 0){
		status = BOOT_SERVICES->AllocatePool(EfiLoaderCode, fileSize,
			&programData);
		efi_assert(status, L"load:segment:AllocatePool");

		status = file->Read(file, &fileSize, programData);
		efi_assert(status, L"load:segment:Read");

		BOOT_SERVICES->CopyMem((void *)physicalAddress, programData, fileSize);
		efi_assert(status, L"load:segment:CopyMem");

		status = BOOT_SERVICES->FreePool(programData);
		efi_assert(status, L"load:segment:FreePool");
	}

	/*
	 * As per ELF Standard, if the size in memory is larger than the file
	 * size the segment is mandated to be zero filled.
	 * For more information on Refer to ELF standard page 34.
	 */
	EFI_PHYSICAL_ADDRESS zeroFillStart = physicalAddress + fileSize;
	UINTN zeroFillCount = memorySize - fileSize;

	if (zeroFillCount > 0)
		BOOT_SERVICES->SetMem((void *)zeroFillStart, zeroFillCount, 0);
}

/*
 * @brief Loads the ELF program segments.
 * Loads the Kernel ELF binary's program segments into memory.
 * @param[in] file			The Kernel EFI file entity to read from.
 * @param[in] headerBuffer		The Kernel header buffer.
 * @param[in] programHeaderBuffer	The kernel program headers buffer.
 */
static void
load_program_segments(EFI_FILE_PROTOCOL * const file,
	Elf_Hdr * const headerBuffer, Elf_Phdr * const programHeaderBuffer)
{
	UINT16 nSegmentsLoaded = 0;
	const UINT16 nProgramHeaders = headerBuffer->e_phnum;

	for (UINT16 p = 0; p < nProgramHeaders; p++){
		if (programHeaderBuffer[p].p_type == PT_LOAD){
			load_segment(file,
				programHeaderBuffer[p].p_offset,
				programHeaderBuffer[p].p_filesz,
				programHeaderBuffer[p].p_memsz,
				programHeaderBuffer[p].p_paddr);

			nSegmentsLoaded++;
		}
	}

#ifdef DEBUG
	if (!nProgramHeaders)
		err_handle(EFI_LOAD_ERROR, L"load:program_segments:phnum");
	else if (!nSegmentsLoaded)
		err_handle(EFI_LOAD_ERROR, L"load:program_segments:loadnum");
#endif
}

EFI_PHYSICAL_ADDRESS
load_kernel(EFI_FILE_PROTOCOL * const root, CHAR16 * const filename)
{
	EFI_STATUS status;
	EFI_FILE_PROTOCOL *file;
	EFI_PHYSICAL_ADDRESS entryPoint;

	Elf_Hdr		*kernelHeader;
	Elf_Phdr	*kernelProgramHeaders;

	status = root->Open(root, &file, filename, EFI_FILE_MODE_READ,
		EFI_FILE_READ_ONLY);
	efi_assert(status, L"load:kernel:Open");

#ifdef DEBUG
	elf_validate(file);
#endif

	// Read the ELF file and program headers.
	elf_read_file(file, &kernelHeader, &kernelProgramHeaders);

	// Set the kernel entry point to the address specified in the ELF header.
	entryPoint = kernelHeader->e_entry;

	load_program_segments(file, kernelHeader, kernelProgramHeaders);

	status = file->Close(file);
	efi_assert(status, L"load:kernel:Close");

	elf_free(kernelHeader, kernelProgramHeaders);

	return entryPoint;
}

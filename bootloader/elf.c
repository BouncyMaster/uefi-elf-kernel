/*
 * Contains functionality to assist in loading and validating ELF executable
 * files. This functionality is essential to the ELF executable loader.
 */

#include "efilib.h"

// TODO: ugly code
void
elf_read_file(EFI_FILE_PROTOCOL * const file, void **headerBuffer,
	void **programHeaderBuffer)
{
	// Read file header
	// TODO: move this to the header
	UINTN bufferReadSize = sizeof(Elf64_Ehdr);
	EFI_STATUS status;

	status = file->SetPosition(file, 0);
	efi_assert(status, L"efi:read_file:header:SetPosition");

	// We don't need to dynamically allocate this if it can only be one type
	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, bufferReadSize,
		headerBuffer);
	efi_assert(status, L"efi:read_file:header:AllocatePool");

	status = file->Read(file, &bufferReadSize, *headerBuffer);
	efi_assert(status, L"efi:read_file:header:Read");

	UINTN programHeaderOffset = ((Elf64_Ehdr*)*headerBuffer)->e_phoff;
	bufferReadSize = sizeof(Elf64_Phdr) *
		((Elf64_Ehdr*)*headerBuffer)->e_phnum;

	// Read program header
	status = file->setPosition(file, programHeaderOffset);
	efi_assert(status, L"efi:read_file:programHeader:SetPosition");

	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, bufferReadSize,
		programHeaderBuffer);
	efi_assert(status, L"efi:read_file:programHeader:AllocatePool");

	status = file->Read(file, &bufferReadSize, *programHeaderBuffer);
	efi_assert(status, L"efi:read_file:programHeader:Read");
}

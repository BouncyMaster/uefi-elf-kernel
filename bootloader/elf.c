#include "efilib.h"
#include "elf.h"

void
elf_read_file(EFI_FILE_PROTOCOL * const file, void **headerBuffer,
	void **programHeaderBuffer)
{
	EFI_STATUS status;
	UINTN bufferReadSize;

	// Read file header
	bufferReadSize = sizeof(Elf64_Ehdr);

	status = file->SetPosition(file, 0);
	efi_assert(status, L"elf:read_file:header:SetPosition");

	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, bufferReadSize,
		headerBuffer);
	efi_assert(status, L"elf:read_file:header:AllocatePool");

	status = file->Read(file, &bufferReadSize, *headerBuffer);
	efi_assert(status, L"elf:read_file:header:Read");

	// Read program header
	UINTN programHeaderOffset = ((Elf64_Ehdr*)*headerBuffer)->e_phoff;
	bufferReadSize = sizeof(Elf64_Phdr) *
		((Elf64_Ehdr*)*headerBuffer)->e_phnum;

	status = file->setPosition(file, programHeaderOffset);
	efi_assert(status, L"elf:read_file:programHeader:SetPosition");

	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, bufferReadSize,
		programHeaderBuffer);
	efi_assert(status, L"elf:read_file:programHeader:AllocatePool");

	status = file->Read(file, &bufferReadSize, *programHeaderBuffer);
	efi_assert(status, L"elf:read_file:programHeader:Read");
}

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

#ifdef DEBUG
UINT8 *
elf_read_identity(EFI_FILE_PROTOCOL * const file)
{
	UINTN bufferReadSize = EI_NIDENT;
	UINT8 *buffer;
	EFI_STATUS status;

	// Reset to the start of the file.
	status = file->SetPosition(file, 0);
	efi_assert(status, L"elf:read_identity:SetPosition");

	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, EI_NIDENT,
		(void**)&buffer);
	efi_assert(status, L"elf:read_identity:AllocatePool");

	status = file->Read(file, &bufferReadSize, (void*)buffer);
	efi_assert(status, L"elf:read_identity:Read");
}

void
elf_validate(UINT8 * const buffer)
{
	if ((elf_identity_buffer[EI_MAG0] != 0x7F) ||
			(elf_identity_buffer[EI_MAG1] != 0x45) ||
			(elf_identity_buffer[EI_MAG2] != 0x4C) ||
			(elf_identity_buffer[EI_MAG3] != 0x46))
		err_handle(EFI_LOAD_ERROR, L"elf:validate:magic");

	if (elf_identity_buffer[EI_CLASS] != ELF_FILE_CLASS_64)
		err_handle(EFI_UNSUPPORTED, L"elf:validate:class");

	if(elf_identity_buffer[EI_DATA] != 1)
		err_handle(EFI_UNSUPPORTED, L"elf:validate:LSB");
}
#endif

#include "efilib.h"
#include "elf.h"

#ifdef DEBUG
void
elf_validate(EFI_FILE_PROTOCOL * const file)
{
	UINTN bufferReadSize = EI_NIDENT;
	UINT8 buffer[EI_NIDENT];
	EFI_STATUS status;

	// Reset to the start of the file.
	status = file->SetPosition(file, 0);
	efi_assert(status, L"elf:validate:SetPosition");

	status = file->Read(file, &bufferReadSize, buffer);
	efi_assert(status, L"elf:validate:Read");

	if ((buffer[EI_MAG0] != 0x7F) || (buffer[EI_MAG1] != 'E') ||
			(buffer[EI_MAG2] != 'L') || (buffer[EI_MAG3] != 'F'))
		err_handle(EFI_LOAD_ERROR, L"elf:validate:magic");

	if (buffer[EI_CLASS] != ELF_FILE_CLASS_64)
		err_handle(EFI_UNSUPPORTED, L"elf:validate:class");

	if(buffer[EI_DATA] != 1)
		err_handle(EFI_UNSUPPORTED, L"elf:validate:LSB");
}
#endif
 
void
elf_read_file(EFI_FILE_PROTOCOL * const file, Elf_Hdr *headerBuffer,
	Elf_Phdr **programHeaderBuffer)
{
	EFI_STATUS status;
	UINTN bufferReadSize;

	// Read file header
	bufferReadSize = sizeof(Elf_Hdr);

	status = file->SetPosition(file, 0);
	efi_assert(status, L"elf:read_file:header:SetPosition");

	status = file->Read(file, &bufferReadSize, headerBuffer);
	efi_assert(status, L"elf:read_file:header:Read");

	// Read program header
	UINTN programHeaderOffset = headerBuffer->e_phoff;
	bufferReadSize = sizeof(Elf_Phdr) * headerBuffer->e_phnum;

	status = file->SetPosition(file, programHeaderOffset);
	efi_assert(status, L"elf:read_file:programHeader:SetPosition");

	status = BOOT_SERVICES->AllocatePool(EfiLoaderData, bufferReadSize,
		(void **)programHeaderBuffer);
	efi_assert(status, L"elf:read_file:programHeader:AllocatePool");

	status = file->Read(file, &bufferReadSize, *programHeaderBuffer);
	efi_assert(status, L"elf:read_file:programHeader:Read");
}

void
elf_free(Elf_Phdr *programHeaderBuffer)
{
	EFI_STATUS status;

	status = BOOT_SERVICES->FreePool(programHeaderBuffer);
	efi_assert(status, L"elf:free:programHeaderBuffer");
}

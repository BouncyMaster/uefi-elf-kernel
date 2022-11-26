#include "efilib.h"

EFI_HANDLE		IMAGE_HANDLE;
EFI_SYSTEM_TABLE *	SYSTEM_TABLE;
EFI_BOOT_SERVICES *	BOOT_SERVICES;

void
efi_init(EFI_HANDLE image, EFI_SYSTEM_TABLE *table)
{
	IMAGE_HANDLE = image;
	SYSTEM_TABLE = table;
	BOOT_SERVICES = table->BootServices;
}

void
err_handle(EFI_STATUS status, CHAR16 *info)
{
	EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *conOut = SYSTEM_TABLE->ConOut;

	conOut->OutputString(conOut, info);
	conOut->OutputString(conOut, L"\r\n");
	BOOT_SERVICES->Exit(IMAGE_HANDLE, status, 0, 0);
}

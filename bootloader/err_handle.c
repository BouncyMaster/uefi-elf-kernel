#include <efi.h>

EFI_HANDLE		IMAGE_HANDLE;
EFI_SYSTEM_TABLE *	SYSTEM_TABLE;

void
err_handle_init(EFI_HANDLE image, EFI_SYSTEM_TABLE *table)
{
	IMAGE_HANDLE = image;
	SYSTEM_TABLE = table;
}

void
err_handle(EFI_STATUS status, CHAR16 *info)
{
	SYSTEM_TABLE->ConOut->OutputString(SYSTEM_TABLE->ConOut, info);
	SYSTEM_TABLE->BootServices->Exit(IMAGE_HANDLE, status, 0, 0);
}

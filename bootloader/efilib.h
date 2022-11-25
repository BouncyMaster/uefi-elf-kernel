#ifndef EFILIB_H
#define EFILIB_H

#include <efi.h>

// Global initialization
extern EFI_HANDLE		IMAGE_HANDLE;
extern EFI_SYSTEM_TABLE *	SYSTEM_TABLE;
extern EFI_BOOT_SERVICES *	BOOT_SERVICES;

void efi_init(EFI_HANDLE image, EFI_SYSTEM_TABLE *table);
void err_handle(EFI_STATUS status, CHAR16 *info);

#endif // EFILIB_H

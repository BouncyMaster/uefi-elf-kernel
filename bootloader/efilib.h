#ifndef EFILIB_H
#define EFILIB_H

#include <efi.h>

// Global initialization
extern EFI_HANDLE		IMAGE_HANDLE;
extern EFI_SYSTEM_TABLE *	SYSTEM_TABLE;
extern EFI_BOOT_SERVICES *	BOOT_SERVICES;

void efi_init(EFI_HANDLE image, EFI_SYSTEM_TABLE *table);
void err_handle(EFI_STATUS status, CHAR16 *info);

#ifdef DEBUG
#define efi_assert(status, info) \
	if (EFI_ERROR(status)) err_handle(status, info)
#else
#define efi_assert(status, info)
#endif // DEBUG

#endif // EFILIB_H

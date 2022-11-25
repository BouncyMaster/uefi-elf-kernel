#ifndef ERR_HANDLE_H
#define ERR_HANDLE_H

#include <efi.h>

void err_handle_init(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table);

void err_handle(EFI_STATUS status, CHAR16 *description);

#endif // ERR_HANDLE_H

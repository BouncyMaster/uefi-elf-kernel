/*
 * ORIGINALLY FROM:
 * @file fs.c
 * @author ajxs
 * @date Aug 2019
 * @brief Filesystem functionality.
 * Contains functionality for interacting with the filesystem via EFI services.
 */

#include "efilib.h"

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *
fs_init(void)
{
	EFI_STATUS status;
	EFI_GUID fsGuid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
	EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *protocol;

	status = BOOT_SERVICES->LocateProtocol(&fsGuid, 0, (void **)&protocol);
	efi_assert(status, L"fs:init");

	return protocol;
}

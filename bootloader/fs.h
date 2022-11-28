// Contains functionality for initiating and working with the file system service.

#ifndef FS_H
#define FS_H

#include "efilib.h"

/**
 * @brief Initialise the file system service.
 * Initialises the UEFI simple file system service, used for interacting with
 * the file system.
 * Refer to: https://mjg59.dreamwidth.org/18773.html?thread=768085#cmt768085
 * @return the FSP pointer.
 */
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL * fs_init(void);

#endif // FS_H

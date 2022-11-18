#include <efi.h>

EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE *SystemTable)
{
	// This clears the screen and buffer.
	SystemTable->ConOut->ClearScreen(SystemTable->ConOut);

	// This prints "Testing..." to the screen ( A.K.A. ConOut is Console Out )
	SystemTable->ConOut->OutputString(SystemTable->ConOut, L"Testing...\r\n");

	// We use this while loop to hang.
	while(1){};

	/*
	 * The EFI needs to have a 0 ( or EFI_SUCCESS ) in order to know 
	 * everything is ok.
	 */
	return EFI_SUCCESS;
}
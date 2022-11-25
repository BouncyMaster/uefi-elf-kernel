#include <efi.h>

#define strcpy(dest, src)		\
	const CHAR16 *_src = (src);	\
	CHAR16 *_dest = (dest);		\
	while ((*_dest++ = *_src++));

EFI_HANDLE		IMAGE_HANDLE;
EFI_SYSTEM_TABLE *	SYSTEM_TABLE;

void
err_handle_init(EFI_HANDLE image, EFI_SYSTEM_TABLE *table)
{
	IMAGE_HANDLE = image;
	SYSTEM_TABLE = table;
}

/*
 * As Exit() needs to be passed a buffer allocated by AllocatePool(), we first
 * allocate it with that function, then copy the string from description to our
 * new buffer.
 */
void
err_handle(EFI_STATUS status, UINTN descriptionSize, CHAR16 *description)
{
	EFI_BOOT_SERVICES *BS = SYSTEM_TABLE->BootServices;
	void *buffer;

	// Not sure if EfiBootServicesData memory type is best to use here.
	BS->AllocatePool(EfiBootServicesData, descriptionSize, &buffer);
	strcpy(buffer, description);
	BS->Exit(IMAGE_HANDLE, status, descriptionSize, buffer);
}

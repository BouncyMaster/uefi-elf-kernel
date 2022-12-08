#include "efilib.h"

void
load_kernel(EFI_FILE_PROTOCOL * const root, CHAR16 * const filename,
	EFI_PHYSICAL_ADDRESS *entryPoint)
{
	EFI_STATUS status;
	EFI_FILE_PROTOCOL *kernel;

	void *kernelHeader = 0; // Don't initialize to 0?
	void *kernelProgramHeaders = 0;

	UINT8 *elfIdentityBuffer = 0;
	Elf_File_Class fileClass = ELF_FILE_CLASS_NONE; // Set as default?

	status = uefi_call_wrapper(root_file_system->Open, 5,
		root_file_system, &kernel_img_file, kernel_image_filename,
		EFI_FILE_MODE_READ, EFI_FILE_READ_ONLY);
	if(check_for_fatal_error(status, L"Error opening kernel file")) {
		return status;
	}

	// Read ELF Identity.
	// From here we can validate the ELF executable, as well as determine the
	// file class.
	status = read_elf_identity(kernel_img_file, &elf_identity_buffer);
	if(check_for_fatal_error(status, L"Error reading executable identity")) {
		return status;
	}

	file_class = elf_identity_buffer[EI_CLASS];

	// Validate the ELF file.
	status = validate_elf_identity(elf_identity_buffer);
	if(EFI_ERROR(status)) {
		// Error message printed in validation function.
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: ELF header is valid\n");
	#endif

	// Free identity buffer.
	status = uefi_call_wrapper(gBS->FreePool, 1, elf_identity_buffer);
	if(check_for_fatal_error(status, L"Error freeing kernel identity buffer")) {
		return status;
	}


	// Read the ELF file and program headers.
	status = read_elf_file(kernel_img_file, file_class,
		&kernel_header, &kernel_program_headers);
	if(check_for_fatal_error(status, L"Error reading ELF file")) {
		return status;
	}

	#ifdef DEBUG
		print_elf_file_info(kernel_header, kernel_program_headers);
	#endif

	// Set the kernel entry point to the address specified in the ELF header.
	if(file_class == ELF_FILE_CLASS_32) {
		*kernel_entry_point = ((Elf32_Ehdr*)kernel_header)->e_entry;
	} else if(file_class == ELF_FILE_CLASS_64) {
		*kernel_entry_point = ((Elf64_Ehdr*)kernel_header)->e_entry;
	}

	status = load_program_segments(kernel_img_file, file_class,
		kernel_header, kernel_program_headers);
	if(EFI_ERROR(status)) {
		// In the case that loading the kernel segments failed, the error message will
		// have already been printed.
		return status;
	}

	// Cleanup.
	#ifdef DEBUG
		debug_print_line(L"Debug: Closing kernel binary\n");
	#endif

	status = uefi_call_wrapper(kernel_img_file->Close, 1, kernel_img_file);
	if(check_for_fatal_error(status, L"Error closing kernel image")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Freeing kernel header buffer\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, (VOID*)kernel_header);
	if(check_for_fatal_error(status, L"Error freeing kernel header buffer")) {
		return status;
	}

	#ifdef DEBUG
		debug_print_line(L"Debug: Freeing kernel program header buffer\n");
	#endif

	status = uefi_call_wrapper(gBS->FreePool, 1, (VOID*)kernel_program_headers);
	if(check_for_fatal_error(status, L"Error freeing kernel program headers buffer")) {
		return status;
	}


	return status;
}

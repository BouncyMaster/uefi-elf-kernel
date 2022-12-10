/*
 * Contains functionality to assist in loading and validating ELF executable
 * files. This functionality is essential to the ELF executable loader.
 * NOTE: only supports 64bit binaries.
 */

#ifndef ELF_H
#define ELF_H

#include "efilib.h"

#define EI_NIDENT	16

#define EI_MAG0		0
#define EI_MAG1		0x1
#define EI_MAG2		0x2
#define EI_MAG3		0x3
#define EI_CLASS	0x4
#define EI_DATA		0x5
#define EI_VERSION	0x6
#define EI_OSABI	0x7

#define PT_LOAD 1

#define ELF_FILE_CLASS_64 2

// The ELF header
typedef struct {
	UINT8	e_ident[EI_NIDENT];
	UINT16	e_type;
	UINT16	e_machine;
	UINT32	e_version;
	UINT64	e_entry;
	UINT64	e_phoff;
	UINT64	e_shoff;
	UINT32	e_flags;
	UINT16	e_ehsize;
	UINT16	e_phentsize;
	UINT16	e_phnum;
	UINT16	e_shentsize;
	UINT16	e_shnum;
	UINT16	e_shstrndx;
} Elf_Hdr;

// The ELF program header
typedef struct {
	UINT32	p_type;
	UINT32	p_flags;
	UINT64	p_offset;
	UINT64	p_vaddr;
	UINT64	p_paddr;
	UINT64	p_filesz;
	UINT64	p_memsz;
	UINT64	p_align;
} Elf_Phdr;


#ifdef DEBUG
/*
 * @brief Validates the ELF file identity.
 * Reads and validates whether the ELF identity correctly identifies an ELF file.
 * @param[in] file	The Kernel EFI file entity to read from.
 */
void elf_validate(EFI_FILE_PROTOCOL * const file);
#endif

/*
 * @brief Reads the ELF file headers.
 * Reads the ELF file header and program headers into memory.
 * @param[in]  file			The Kernel EFI file entity to read from.
 * @param[out] headerBuffer		The buffer to read the kernel header into.
 * @param[out] programHeaderBuffer	The buffer to read the kernel program
 *					headers into.
 */
void elf_read_file(EFI_FILE_PROTOCOL * const file, Elf_Hdr **headerBuffer,
	Elf_Phdr **programHeaderBuffer);

void elf_free(Elf_Hdr *headerBuffer, Elf_Phdr *programHeaderBuffer);

#endif // ELF_H

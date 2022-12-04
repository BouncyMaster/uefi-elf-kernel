/*
 * Contains functionality to assist in loading and validating ELF executable
 * files. This functionality is essential to the ELF executable loader.
 */

#ifndef ELF_H
#define ELF_H

#include "efilib.h"

#define EI_NIDENT        16

#define EI_MAG0          0
#define EI_MAG1          0x1
#define EI_MAG2          0x2
#define EI_MAG3          0x3
#define EI_CLASS         0x4
#define EI_DATA          0x5
#define EI_VERSION       0x6
#define EI_OSABI         0x7
#define EI_ABIVERSION    0x8

#define PT_NULL            0
#define PT_LOAD            1
#define PT_DYNAMIC         2
#define PT_INTERP          3
#define PT_NOTE            4
#define PT_SHLIB           5
#define PT_PHDR            6
#define PT_TLS             7

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
} Elf_hdr;

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

/*
 * @brief Reads the ELF file headers.
 * Reads the ELF file header and program headers into memory.
 * @param[in]  file			The Kernel EFI file entity to read from.
 * @param[out] headerBuffer		The buffer to read the kernel header into.
 * @param[out] programHeaderBuffer	The buffer to read the kernel program
 *					headers into.
 */
void elf_read_file(EFI_FILE_PROTOCOL * const file, void **headerBuffer,
	void **programHeaderBuffer);

#ifdef DEBUG
/*
 * @brief Reads the identity buffer of an ELF file.
 * Reads the identity buffer from the ELF header, which is used to validate
 * that the file is a valid ELF executable.
 * @param[in] file	The kernel binary to read the file class from.
 *
 * @return The identity buffer.
 */
UINT8 * elf_read_identity(EFI_FILE_PROTOCOL * const file);

/*
 * @brief Validates the ELF file identity.
 * Validates whether the ELF identity correctly identifies an ELF file.
 * @param[in] buffer	The ELF identity buffer to validate.
 */
void elf_validate(UINT8 * const buffer);
#endif

#endif // ELF_H

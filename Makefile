BOOTLOADER_CC = x86_64-w64-mingw32-gcc
BOOTLOADER_INCS = -I../include
BOOTLOADER_SRC = bootloader/main.c bootloader/graphics.c bootloader/efilib.c \
	bootloader/fs.c bootloader/elf.c bootloader/loader.c
BOOTLOADER_CFLAGS = -Wall -Wno-unused-but-set-variable -Wvla -Werror -m64 -mabi=ms \
	-ffreestanding -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main
BOOTLOADER_BIN = bootloader.efi

KERNEL_CC = x86_64-elf-gcc
KERNEL_INCS = -Ikernel/include
KERNEL_SRC = kernel/main.c kernel/graphics.c
KERNEL_CFLAGS = -Wall -Wvla -Werror -ffreestanding -nostdlib \
	-z max-page-size=0x1000
KERNEL_BIN = kernel.elf

DBG_CFLAGS = -DDEBUG -g
REL_CFLAGS = -O2

dbg_bootloader: $(BOOTLOADER_SRC)
	$(BOOTLOADER_CC) -o drive/$(BOOTLOADER_BIN) $(BOOTLOADER_SRC) $(BOOTLOADER_CFLAGS) $(DBG_CFLAGS) $(BOOTLOADER_INCS)

rel_bootloader: $(BOOTLOADER_SRC)
	$(BOOTLOADER_CC) -o drive/$(BOOTLOADER_BIN) $(BOOTLOADER_SRC) $(BOOTLOADER_CFLAGS) $(REL_CFLAGS) $(BOOTLOADER_INCS)

dbg_kernel: $(KERNEL_SRC)
	$(KERNEL_CC) -T kernel/kernel.ld -o drive/$(KERNEL_BIN) $(KERNEL_SRC) $(KERNEL_CFLAGS) $(DBG_CFLAGS) $(KERNEL_INCS)

rel_kernel: $(KERNEL_SRC)
	$(KERNEL_CC) -T kernel/kernel.ld -o drive/$(KERNEL_BIN) $(KERNEL_SRC) $(KERNEL_CFLAGS) $(REL_CFLAGS) $(KERNEL_INCS)

tags:
	cd bootloader && ctags -R . && cd ..
	cd kernel && ctags -R . && cd ..

run:
	qemu-system-x86_64 -net none -bios /usr/share/edk2-ovmf/OVMF_CODE.fd -drive format=raw,file=fat:rw:drive/

clean:
	rm -f drive/$(BOOTLOADER_BIN) drive/$(KERNEL_BIN) bootloader/tags kernel/tags

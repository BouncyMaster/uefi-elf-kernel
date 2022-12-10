BOOTLOADER_CC = x86_64-w64-mingw32-gcc
BOOTLOADER_INCS = -I../include
BOOTLOADER_SRC = bootloader/main.c bootloader/graphics.c bootloader/efilib.c \
	bootloader/fs.c
BOOTLOADER_CFLAGS = -Wall -Wno-unused-but-set-variable -Wvla -Werror -m64 -mabi=ms \
	-ffreestanding -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main
BOOTLOADER_BIN = bootloader.efi

DBG_CFLAGS = -DDEBUG -g
REL_CFLAGS = -O2

dbg_bootloader: $(BOOTLOADER_SRC)
	$(BOOTLOADER_CC) -o drive/$(BOOTLOADER_BIN) $(BOOTLOADER_SRC) $(BOOTLOADER_CFLAGS) $(DBG_CFLAGS) $(BOOTLOADER_INCS)

rel_bootloader: $(BOOTLOADER_SRC)
	$(BOOTLOADER_CC) -o drive/$(BOOTLOADER_BIN) $(BOOTLOADER_SRC) $(BOOTLOADER_CFLAGS) $(REL_CFLAGS) $(BOOTLOADER_INCS)

tags:
	cd bootloader && ctags -R . && cd ..

run:
	qemu-system-x86_64 -net none -bios /usr/share/edk2-ovmf/OVMF_CODE.fd -drive format=raw,file=fat:rw:drive/

clean:
	rm -f drive/$(BOOTLOADER_BIN) bootloader/tags

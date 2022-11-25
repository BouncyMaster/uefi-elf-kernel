BOOTLOADER_CC = x86_64-w64-mingw32-gcc
BOOTLOADER_INCS = -I../include
BOOTLOADER_SRC = bootloader/main.c bootloader/efilib.c
BOOTLOADER_CFLAGS = -Wall -Werror -m64 -mabi=ms -ffreestanding -nostdlib -shared \
	-Wl,-dll -Wl,--subsystem,10 -e efi_main
BOOTLOADER_BIN = bootloader.efi

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
	$(BOOTLOADER_CC) -o drive/$@ $(BOOTLOADER_SRC) $(BOOTLOADER_CFLAGS) $(BOOTLOADER_INCS)

tags:
	ctags `find . -name "*.c"`

run: $(BOOTLOADER_BIN)
	qemu-system-x86_64 -net none -bios /usr/share/edk2-ovmf/OVMF_CODE.fd -drive format=raw,file=fat:rw:drive/

clean:
	rm -f drive/$(BOOTLOADER_BIN)

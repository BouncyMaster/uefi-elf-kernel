CC = x86_64-w64-mingw32-gcc
INCS = -I../include
CFLAGS = -Wall -Werror -m64 -mabi=ms -ffreestanding -nostdlib -shared -Wl,-dll \
	-Wl,--subsystem,10 -e efi_main
SRC = bootloader/main.c

bootloader.efi: $(SRC)
	$(CC) -o drive/$@ $^ $(CFLAGS) $(INCS)

run: bootloader.efi
	qemu-system-x86_64 -net none -bios /usr/share/edk2-ovmf/OVMF_CODE.fd -drive format=raw,file=fat:rw:drive/

clean:
	rm drive/bootloader.efi

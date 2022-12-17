CC = x86_64-w64-mingw32-gcc
INCS = -Iinclude
SRC = main.c graphics.c efilib.c fs.c elf.c loader.c
CFLAGS = -Wall -Wno-unused-but-set-variable -Wvla -Werror -m64 -mabi=ms \
	-ffreestanding -nostdlib -shared -Wl,-dll -Wl,--subsystem,10 -e efi_main
BIN = ../drive/bootloader.efi

DBG_CFLAGS = -DDEBUG -g
REL_CFLAGS = -O2

dbg: $(SRC)
	$(CC) -o $(BIN) $^ $(CFLAGS) $(DBG_CFLAGS) $(INCS)

rel: $(SRC)
	$(CC) -o $(BIN) $^ $(CFLAGS) $(REL_CFLAGS) $(INCS)

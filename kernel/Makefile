CC = x86_64-elf-gcc
INCS = -Iinclude
SRC = main.c print.c
CFLAGS = -Wall -Wvla -Wno-char-subscripts -ffreestanding -nostdlib \
	-T kernel.ld -z max-page-size=0x1000
BIN = ../drive/kernel.elf

DBG_CFLAGS = -DDEBUG -g
REL_CFLAGS = -O2

font.o: font.sfn
	objcopy -O elf64-x86-64 -B i386 -I binary $^ $@

dbg: $(SRC) font.o
	$(CC) -o $(BIN) $^ $(CFLAGS) $(DBG_CFLAGS) $(INCS)

rel: $(SRC) font.o
	$(CC) -o $(BIN) $^ $(CFLAGS) $(REL_CFLAGS) $(INCS)

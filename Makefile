tags:
	cd bootloader && ctags -R . && cd ..
	cd kernel && ctags -R . && cd ..

run:
	qemu-system-x86_64 -net none -bios /usr/share/edk2-ovmf/OVMF_CODE.fd -drive format=raw,file=fat:rw:drive/

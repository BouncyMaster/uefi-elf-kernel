@echo -off

bootloader.efi

If not %lasterror% == 0 then
	echo "[Error] LastError = %lasterror%"
else
	echo "[OK] Status = 0 [Success]"
endif

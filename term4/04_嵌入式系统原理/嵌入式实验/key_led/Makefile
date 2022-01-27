key.bin: start.o main.o
	arm-linux-ld -Ttext 0x50000000 -o key.elf $^
	arm-linux-objcopy -O binary key.elf key.bin
	arm-linux-objdump -D key.elf > key_elf.dis
%.o : %.S
	arm-linux-gcc -o $@ $< -c

%.o : %.c
	arm-linux-gcc -o $@ $< -c 

clean:
	rm *.o *.elf *.bin *.dis  -rf
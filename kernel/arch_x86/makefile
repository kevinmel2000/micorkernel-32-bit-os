NASM=nasm -f elf -o
GCC=gcc -I ../../include -c -o
arch_x86:
	cd interrupt && make interrupt 
	$(NASM) ../../arch_x86_s.o arch_x86.asm
	$(GCC) ../../arch_x86_c.o arch_x86.c
	$(GCC) ../../chipset.o chipset.c
GCC=gcc -I ../../../include -c -o 
NASM=nasm -f elf -o
interrupt:
	$(GCC) ../../../interrupt.o interrupt.c
	$(NASM) ../../../intfunction.o intfunction.asm
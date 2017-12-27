NO_STDLIB=-nostdlib -fno-builtin
GCC=gcc -I ../include/ $(NO_STDLIB) -c -o
NASM=nasm -f elf -o

kernel:
	$(GCC) ../myos.o myos.c
	$(GCC) ../task.o task.c
	$(GCC) ../memory.o memory.c
	$(GCC) ../string.o string.c
	$(GCC) ../signal_c.o signal.c
	$(NASM) ../signal_s.o signal.asm
	cd arch_x86 && make arch_x86
	cd IPC && make message
	cd process_manage && make pm
	cd memory_manage && make mm
	cd init && make init

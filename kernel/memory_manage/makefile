NO_STDLIB=-nostdlib -fno-builtin
GCC=gcc -I ../../include/ $(NO_STDLIB) -c -o

mm:
	$(GCC) ../../mm.o memory_manage.c
	$(GCC) ../../malloc.o malloc.c
	$(GCC) ../../remap.o remap.c

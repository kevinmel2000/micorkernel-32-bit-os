NO_STDLIB=-nostdlib -fno-builtin
GCC=gcc -I ../../include/ $(NO_STDLIB) -c -o

pm:
	$(GCC) ../../pm.o process_manage.c
	$(GCC) ../../fork.o fork.c
	$(GCC) ../../exit.o exit.c

GCC=gcc -fno-builtin -I include/
NASM=nasm -f elf -o 
CFLAGS=-fno -stack -protector
.PHONY:lib app os
all:lib app os
os:
	#mkdir fd
	#cd driver && make hd
	#cd fs && make fs
	#cd console && make console
	#cd interrupt && make interrupt
	#cd IPC && make IPC
	cd kernel && make kernel
	#cd keyboard && make keyboard
	#cd string && make string
	#cd tty && make tty
	#cd shell && make shell
	#cd MM && make mm 
	#cd init && make init
	#cd tools && make tools
	cd boot &&make boot
	ld myos.o setigdt.o fnction.o memory.o -o sys.bin -Ttext 0x200000 -e main --oformat binary #int.o intfunction.o function.o task.o message.o memory.o console.o console.elf keyboard.o tools.o string.o wordlib.o tty.o hd.o fs.o shell.o mm.o init.o -o kernel.bin -Ttext 0x200000 -e main --oformat binary
	cat asmhead.bin kernel.bin>sys.bin
	mkdir fd
	dd if=/dev/zero of=myos.img bs=1k count=1440
	mkfs -t msdos myos.img
	#mkdir fd
	sudo mount -o loop myos.img fd
	sudo cp scnd.bin fd -v
	#sudo cp pro.elf fd -v
	#sudo cp hdboot.bin fd -v 
	dd if=boot.bin of=myos.img bs=512 count=1 conv=notrunc
	sudo umount fd	
	#rmdir fd
clean:
	rm -f *.o
	rm -f *.bin
	rmdir fd
lib:
	cd libr && make lib
app:	
	cd libr && make app
test:
	cd boot && make boot
	cd kernel && make kernel
	ld myos.o arch_x86_c.o arch_x86_s.o chipset.o interrupt.o intfunction.o memory.o task.o signal_c.o signal_s.o pm.o mm.o init.o message.o fork.o exit.o malloc.o remap.o  string.o -o sys.bin -Ttext 0x200000 -e main --oformat binary
# check the fd file if exsists
	if [ ! -x "fd" ]; then mkdir fd; fi
#############################
	dd if=/dev/zero of=myos.img bs=1k count=1440
	mkfs -t msdos myos.img
	sudo mount -o loop myos.img fd
# copy sys and the driver into the img file
	sudo cp scnd.bin fd -v
	sudo cp sys.bin fd -v
	sudo cp fs.dv fd -v
	sudo cp hd.dv fd -v
	sudo cp key.dv fd -v
	sudo cp shl.dv fd -v
	sudo cp tty.dv fd -v
############################################
	sudo umount fd
# put the boot into the head of img file
	dd if=boot.bin of=myos.img bs=512 count=1 conv=notrunc

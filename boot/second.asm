	cpu 486
	org 0x8000
name_add		equ 0x526
driver_num		equ 0x504
kernel_add 		equ 0x500
kernel_size_add 	equ 0x502
read_add 		equ 0x520
find_by_name_add 	equ 0x524
driver_desc_add 	equ 0x600
FILES			equ 0x1
HD			equ 0x2
TTY			equ 0x4
CONSOLE			equ 0x8
KEYBOARD		equ 0x10
SHELL 			equ 0x20
kernel_pos              equ 0x200000
	jmp start
sys:
	db "SYS     BIN "
files:
	db "FS      DV  "
hd:
	db "HD      DV  "
tty:
	db "TTY     DV  "
keyboard:
	db "KEY     DV  "
shell:
	db "SHL     DV  "
start:
	mov ax,6
	mov [driver_num],ax
	mov ax,[read_add]
	mov [kernel_add],ax
	mov [kernel_size_add],ax
	mov ax,sys
	mov [name_add],ax
	mov ax,[find_by_name_add]
	call ax
	mov ax,[read_add]
	sub ax,[kernel_size_add]
	mov [kernel_size_add],ax
	mov ax,FILES
	mov [driver_desc_add],ax
	mov ax,[read_add]
	mov [driver_desc_add+2],ax
	mov ax,files
	mov [name_add],ax
	mov ax,[find_by_name_add]
	call ax
	mov ax,HD
	mov [driver_desc_add+4],ax
	mov ax,[read_add]
	mov [driver_desc_add+6],ax
	mov ax,hd
	mov [name_add],ax
	mov ax,[find_by_name_add]
	call ax
	mov ax,TTY
	mov [driver_desc_add+8],ax
	mov ax,[read_add]
	mov [driver_desc_add+10],ax
	mov ax,tty
	mov [name_add],ax
	mov ax,[find_by_name_add]
	call ax
	mov ax,KEYBOARD
	mov [driver_desc_add+16],ax
	mov ax,[read_add]
	mov [driver_desc_add+18],ax
	mov ax,keyboard
	mov [name_add],ax
	mov ax,[find_by_name_add]
	call ax
	mov ax,SHELL
	mov [driver_desc_add+20],ax
	mov ax,[read_add]
	mov [driver_desc_add+22],ax
	mov ax,shell
	mov [name_add],ax
	mov ax,[find_by_name_add]
	call ax
	cli         
	mov al,0xff 		;disable the interrupt
	out 0x21,al 
	nop         
	nop         
	out 0xa1,al
	mov ax,0x2401      	;open the A20 GATE
	int 0x15
	lgdt [gdt0]            	;store the temp_gdt to the gdtr register 
	mov eax,cr0
	or eax,1               	;open the x86 protect mode
	mov cr0,eax
	mov eax,0
	jmp pipe
pipe:
	mov ax,1*8            ;the ss,ds,fs,es point to the first selector
	mov ds,ax
	mov ss,ax
	mov es,ax
	mov fs,ax
	mov esp,0x8000
	mov eax,0
	mov ax,[kernel_add]       ;copy the os kernel to the 0x200000
	shl ax,4
	mov esi,eax
	mov eax,0x200000
	mov edi,eax
	mov ecx,0
	mov cx,[kernel_size_add]          ;get the kernel size
	shl ecx,4
	call memcpy             ;do copy
	mov esp,0x8000
	jmp dword 2*8:kernel_pos       	;jmp to the real kernel
	alignb 16
gdt:
	resb 8
	dw 0xffff,0            	;the data segment
	db 0,0x92,0xcf,0
	dw 0xffff,0             ;the code segment
	db 0,0x9a,0xcf,0
gdt0:
	dw 4*8-1
	dd gdt
memcpy:                         ;copy function
	mov eax,[esi]
	mov [edi],eax
	add esi,4
	add edi,4
	sub ecx,4
	jnz memcpy
	ret	

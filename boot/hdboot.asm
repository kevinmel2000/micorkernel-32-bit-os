	cpu 486
	org 0x7c00
	mov ax,cs
	mov ss,ax
	mov sp,0x7200
	mov ax,0x820
	mov es,ax
	mov si,0
	mov ch,0
	mov dh,2
	mov cl,2
read:
	mov ah,2
	mov al,1
	mov dl,0x80
	mov bx,0
	int 0x13
	jc show_error
	inc si
	cmp si,0x7a
	jae over
	mov ax,es
	add ax,0x20
	mov es,ax
read1:
	cmp cl,17
	jae read2
	inc cl
	jmp read
read2:
	cmp dh,3
	jae read3
	inc dh
	mov cl,1
	jmp read
read3:
	cmp ch,0xff
	jae over
	inc ch
	mov dh,0
	mov cl,1
	jmp read
over:
	jmp 0x8200
show_error:
	mov ax,0xb800
	mov es,ax
	mov bx,0
	mov ax,cs
	mov ds,ax
	mov di,error
put_char:
	mov ah,0x7
	mov al,[di]
	cmp al,0
	je die
	mov [es:bx],ax
	inc di
	add bx,2
	jmp put_char
die:
	jmp die
error:
	db "error to load hard drive",0
	
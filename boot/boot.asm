	cpu 486
find_by_name_add equ 0x524
read_add 	 equ 0x520
name_add 	 equ 0x526
temp_add	 equ 0x530
ret_add		 equ 0x536
	org 0x7c00
	jmp enter
	db		0x90
	db		"hellomos"		
	dw		512				
	db		1				
	dw		1				
	db		2				
	dw		224				
	dw		2880			
	db		0xf0			
	dw		9				
	dw		18				
	dw		2				
	dd		0				
	dd		2880			
	db		0,0,0x29		
	dd		0xffffffff		
	db		"hello myos "	
	db		"FAT12   "		
	resb	18
second:
	db "SCND    BIN " 
error:
	db "fail to load",0
enter:
	xor ax,ax		
	mov ss,ax
	mov sp,0x7200     
	mov ax,0x7e0       
	mov es,ax
	xor ax,ax      	
	mov ds,ax
	xor bx,bx  
	mov ch,0
	mov dh,1
	mov cl,2
	mov ah,2
	mov al,1
	mov dl,0
	int 0x13
	mov ax,find_by_name
	mov [find_by_name_add],ax
	mov ax,0x800
	mov [read_add],ax
	mov ax,second
	mov [name_add],ax
	call find_by_name
	jmp 0x8000
read_init:
	mov dx,[temp_add+2]
	mov ax,[temp_add]
	mov bx,0x200
	div bx
	inc ax
	mov si,ax
	mov ax,[temp_add+4]
	add ax,0x1f
	mov bl,0x12
	div bl
	mov cl,ah
	inc cl
	push ax
	and ax,1
	mov dh,al
	pop ax
	shr al,1
	mov ch,al
	mov ax,[read_add]
	mov es,ax
	xor bx,bx   
	xor di,di   	
read:
	mov ah,2
	mov al,1
	mov dl,0
	xor bx,bx	
	int 0x13
	jc again
	mov ax,es
	add ax,0x20
	mov es,ax
	dec si
	cmp si,0
	je read_over
read1:
	cmp cl,18
	jae read2
	inc cl
	jmp read
read2:
	cmp dh,1
	jae read3
	inc dh
	mov cl,1
	jmp read
read3:
	cmp ch,0x30
	inc ch
	mov dh,0
	mov cl,1
	jmp read
again:
	cmp di,5
	je show_error
	inc di
	mov ax,[read_add]
	mov es,ax
	xor bx,bx
	mov ah,0
	mov dl,0
	int 0x13
	mov ch,0
	mov dh,0
	mov cl,2
	jmp read
show_error:
	mov ax,0xb800
	mov es,ax
	xor si,si
	xor ax,ax
	mov ds,ax
	mov bx,error
	mov ah,0x7
show:
	mov al,[bx]
	cmp al,0
	je fin
	mov [es:si],ax
	inc bx
	add si,2
	jmp show
find_by_name:
	mov ax,ss
	mov ds,ax
	mov bx,sp
	mov ax,[bx]
	mov [ret_add],ax
	mov ax,cs
	mov ds,ax
	mov ax,0x7e0
	mov es,ax
	mov bx,[name_add]
	xor dx,dx
	xor cx,cx
find_name:
	mov al,[bx]
	push bx
	mov bx,dx
	mov bl,[es:bx]
	cmp al,bl
	jne not_patch
	pop bx
	inc bx
	inc dx
	inc cx
	cmp cx,0xb
	jae finded
	jmp find_name
not_patch:
	mov bx,[name_add]
	and dx,0xfff0
	add dx,0x20
	xor cx,cx
	jmp find_name
finded:
	and dx,0xfff0
	mov bx,dx
	add bx,0x1a
	mov ax,[es:bx]
	mov [temp_add+4],ax
	mov bx,dx
	add bx,0x1c
	mov ax,[es:bx]
	mov [temp_add],ax
	add bx,2
	mov ax,[es:bx]
	mov [temp_add+2],ax
	jmp read_init
read_over:
	mov ax,es
	mov [read_add],ax
	mov ax,[ret_add]
	push ax
	ret
fin:
	hlt
	jmp fin
	times 510-($-$$) db 0
	db 0x55,0xaa
	

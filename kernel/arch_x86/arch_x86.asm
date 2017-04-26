	cpu 486
[SECTION .text]
	global hlt,lgdt,lidt,out,in,sti,cli,far_jump,ltr,nop
	global check_memory,get_memory,show,set_memory,int40,store_page
	global in_word,out_word,out_dword,in_dword,get_error_add
	global get_esp,fork,reset_stack,reset_selectors
	global play,playb,playc,playd
reset_selectors:
	mov eax,[esp+4]
	lldt ax
	mov eax,[esp+12]
	mov es,ax
	mov ds,ax
	mov gs,ax
	mov fs,ax
	mov ss,ax
	mov eax,[esp+8]
	jmp 1*8+4:next
next:	
	ret
nop:
	nop
	ret
reset_stack:
	mov ebx,[esp+4]
	mov ecx,0x8000
	mov eax,ecx
	mov edx,esp
	sub ecx,edx
	shr ecx,2
copy:
	sub eax,4
	sub ebx,4
	mov edx,[eax]
	mov [ebx],edx
	loop copy
	mov esp,ebx
	add ebx,0x2c
	mov ebp,ebx
	ret
playd:
	mov ebx,[0]
	mov ah,0xf
	mov al,'x'
	mov [ebx],ax
	add ebx,2
	mov [0],ebx
	ret
playb:
	mov ebx,[0]
	mov ah,0x7
	mov al,'b'
	mov [ebx],ax
	add ebx,2
	mov [0],ebx
	ret
play:
	mov ebx,0xb8002
	mov al,'a'
	mov ah,0x7
	mov [ebx],ax
	add ebx,2
	mov [0],ebx
	ret
playc:
	mov ebx,[0]
	mov al,'c'
	mov ah,0xf
	mov [ebx],ax
	add ebx,2
	mov [0],ebx
	ret
store_page:
	push ebp
	mov ebp,esp
	mov eax,[esp+8]
	mov cr3,eax
	mov eax,cr0
	or eax,0x80000001
	mov cr0,eax
	pop ebp
	ret
get_error_add:
	push ebp
	mov ebp,esp
	mov eax,cr2
	pop ebp
	ret
out_dword:
	push ebp
	mov ebp,esp
	mov edx,[esp+8]
	mov eax,[esp+12]
	out dx,eax
	pop ebp
	ret
in_dword:
	push ebp
	mov ebp,esp
	mov edx,[esp+8]
	in eax,dx
	pop ebp
	ret
out_word:
	push ebp
	mov ebp,esp
	mov edx,[esp+8]
	mov eax,[esp+12]
	out dx,ax
	pop ebp
	ret
in_word:
	push ebp
	mov ebp,esp
	mov eax,0
	mov dx,[esp+8]
	in ax,dx
	pop ebp
	ret
int40:
	push ebp
	mov ebp,esp
	mov edx,[esp+8]
	int 0x40
	pop ebp
	ret
ltr:
	ltr [esp+4]
	ret
far_jump:
	jmp far [esp+4]
	ret
set_memory:
	push ebp
	mov ebp,esp
	push eax
	push ebx
	mov ebx,[esp+16]
	mov eax,[esp+20]
	mov [ebx],eax
	pop ebx
	pop eax
	pop ebp
	ret
get_memory:
	push ebp
	mov ebp,esp
	mov ecx,[esp+8]
	mov dword eax,[ecx]
	pop ebp
	ret
check_memory:
	push ebp
	mov ebp,esp
	mov ebx,0
check:
	mov eax,[ebx]
	mov dword [ebx],0x55555555
	xor dword [ebx],0xffffffff
	cmp dword [ebx],0xaaaaaaaa
	jne check_over
	mov [ebx],eax
	add ebx,0x100000
	jmp check
check_over:
	mov eax,ebx
	pop ebp
	ret
cli:
	cli
	ret
sti:
	sti
	ret
in:
	push ebp
	mov ebp,esp
	mov edx,[esp+8]
	mov eax,0
	in al,dx
	movzx eax,al
	pop ebp
	ret
out:
	push ebp
	mov ebp,esp
	mov edx,[esp+8]
	mov eax,[esp+12]
	out dx,al
	pop ebp
	ret
lidt:
	push ebp
	mov ebp,esp
	mov eax,[esp+8]
	mov [esp+10],ax
	lidt [esp+10]
	pop ebp
	ret
lgdt:
	push ebp
	mov ebp,esp
	mov eax,[esp+8]
	mov [esp+10],ax
	lgdt [esp+10]
	pop ebp
	ret

hlt:
	hlt
	ret
	
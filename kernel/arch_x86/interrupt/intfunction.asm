	cpu 486
	;; software exception handler
	extern handle0x00,handle0x05,handle0x07,handle0x08,handle0x09
	extern handle0x10,handle0x11,handle0x12,handle0x13,handle0x14
	extern handle0x16,handle0x17,handle0x18,handle0x19
	;; hardware interrupt handler
	extern handle0x20,handle0x21,handle0x22,handle0x23,handle0x24,handle0x25
	extern handle0x26,handle0x27,handle0x28,handle0x29,handle0x2a,handle0x2b
	extern handle0x2c,handle0x2d,handle0x2e,handle0x2f
	;; software system call handler
	extern sys_send_recv
[section .text]
	;; software exception handler
	global asm_handle0x00
	global asm_handle0x05,asm_handle0x07,asm_handle0x08,asm_handle0x09
	global asm_handle0x10,asm_handle0x11,asm_handle0x12,asm_handle0x13
	global asm_handle0x14,asm_handle0x16,asm_handle0x17,asm_handle0x18
	global asm_handle0x19
	;; hardware interrupt handler
	global asm_handle0x20,asm_handle0x21,asm_handle0x22,asm_handle0x23
	global asm_handle0x24,asm_handle0x25,asm_handle0x26,asm_handle0x27
	global asm_handle0x28,asm_handle0x29,asm_handle0x2a,asm_handle0x2b
	global asm_handle0x2c,asm_handle0x2d,asm_handle0x2e,asm_handle0x2f
	;; software system call handler
	global asm_send_recv

asm_handle0x00:
	push 0
	push handle0x00
	jmp handler
asm_handle0x05:
	push 0
	push handle0x05
	jmp handler
asm_handle0x07:
	push 0
	push handle0x07
	jmp handler
asm_handle0x08:
	push handle0x08
	jmp handler
asm_handle0x09:
	push 0
	push handle0x09
	jmp handler
asm_handle0x10:
	push handle0x10
	jmp handler
asm_handle0x11:
	push handle0x11
	jmp handler
asm_handle0x12:
	push handle0x12
	jmp handler
asm_handle0x13:
	push handle0x13
	jmp handler
asm_handle0x14:
	push handle0x14
	jmp handler
asm_handle0x16:
	push 0
	push handle0x16
	jmp handler
asm_handle0x17:
	push handle0x17
	jmp handler
asm_handle0x18:
	push 0
	push handle0x18
	jmp handler
asm_handle0x19:
	push 0
	push handle0x19
	jmp handler
;;; hardware interrupt handler
asm_handle0x20:
	push handle0x20
	jmp hard_handler
asm_handle0x21:
	push handle0x21
	jmp hard_handler
asm_handle0x22:
	push handle0x22
	jmp hard_handler
asm_handle0x23:
	push handle0x23
	jmp hard_handler
asm_handle0x24:
	push handle0x24
	jmp hard_handler
asm_handle0x25:
	push handle0x25
	jmp hard_handler
asm_handle0x26:
	push handle0x26
	jmp hard_handler
asm_handle0x27:
	push handle0x27
	jmp hard_handler
asm_handle0x28:
	push handle0x28
	jmp hard_handler
asm_handle0x29:
	push handle0x29
	jmp hard_handler
asm_handle0x2a:
	push handle0x2a
	jmp hard_handler
asm_handle0x2b:
	push handle0x2b
	jmp hard_handler
asm_handle0x2c:
	push handle0x2c
	jmp hard_handler
asm_handle0x2d:
	push handle0x2d
	jmp hard_handler
asm_handle0x2e:
	push handle0x2e
	jmp hard_handler
asm_handle0x2f:
	push handle0x2f
	jmp hard_handler
	
	;; system call handler
asm_send_recv:
	cli
	push es
	push ds
	pushad
	mov eax,esp
	mov ax,ss
	mov ds,ax
	mov es,ax
	push edx
	call sys_send_recv
	pop edx
	popad
	pop ds
	pop es
	sti
	iretd
	;; stftware exception handler
	;; if there is error code,then it is there
	;; if not,use '0' to fill this place
handler:
	push es
	push ds
	push eax
	push ecx
	push edx
	push ebx
	push esp
	push ebp
	push esi
	push edi
	mov eax,esp
	push eax
	mov ax,ss
	mov ds,ax
	mov es,ax
	mov ecx,[ss:esp+48]
	mov ebx,[ss:esp+52]
	mov eax,[ss:esp+44]
	push ecx
	push ebx
	call eax
	add esp,12
	pop edi
	pop esi
	pop ebp
	pop esp
	pop ebx
	pop edx
	pop ecx
	pop eax
	pop ds
	pop es
	add esp,8
	iretd
	;; hardware interrupt
hard_handler:
	push es
	push ds
	push eax
	push ecx
	push edx
	push ebx
	push esp
	push ebp
	push esi
	push edi
	mov ax,ss
	mov ds,ax
	mov es,ax
	mov eax,[ss:esp+40]
	call eax
	pop edi
	pop esi
	pop ebp
	pop esp
	pop ebx
	pop edx
	pop ecx
	pop eax
	pop ds
	pop es
	add esp,4
	iretd

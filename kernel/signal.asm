	cpu 486
[section .text]
	global signal_handler_entry
signal_handler_entry:
	call eax
	pop edi
	pop esi
	add esp,8
	pop ebx
	pop edx
	pop ecx
	pop eax
	ret
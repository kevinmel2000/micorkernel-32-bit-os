#include <arch_x86/arch_x86.h>
#include <memory.h>
#include <arch_x86/interrupt/interrupt.h>
void init_gdt_idt(unsigned int kernel_size)
{
	gdt_addr=(struct gdt*)GDT_ADD;
	int i;
	for(i=0;i<GDT_SIZE;i++)
	{
		set_gdt(&gdt_addr[i],0,0,0);
	}
	set_gdt(&gdt_addr[1],0,X86_MAX_SIZE,DEFAULT_DESCRIP|SEGMENT_PRESENT|CODE_DATA_DESCRIP|DATA_DESCRIP);
	set_gdt(&gdt_addr[2],0,X86_MAX_SIZE,DEFAULT_DESCRIP|SEGMENT_PRESENT|CODE_DATA_DESCRIP|CODE_DESCRIP);
	lgdt(GDT_LENGTH,GDT_ADD);
	free_memory_add=(GDT_ADD+GDT_SIZE*sizeof(struct gdt));
	if((GDT_ADD+GDT_SIZE*sizeof(struct gdt))&0xf!=0)
	  {
	    free_memory_add=(((GDT_ADD+GDT_SIZE*sizeof(struct gdt))&0xfffffff0)+8);
	  }
	idt_addr=(struct idt*)free_memory_add;
	for(i=0;i<IDT_SIZE;i++)
	{
		set_idt(&idt_addr[i],0,0,0);
	}
	set_idt(&idt_addr[0x00],(unsigned int)asm_handle0x00,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x05],(unsigned int)asm_handle0x05,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x07],(unsigned int)asm_handle0x07,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x08],(unsigned int)asm_handle0x08,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x09],(unsigned int)asm_handle0x09,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x10],(unsigned int)asm_handle0x10,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x11],(unsigned int)asm_handle0x11,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x12],(unsigned int)asm_handle0x12,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x13],(unsigned int)asm_handle0x13,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x14],(unsigned int)asm_handle0x14,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x16],(unsigned int)asm_handle0x16,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x17],(unsigned int)asm_handle0x17,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x18],(unsigned int)asm_handle0x18,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x19],(unsigned int)asm_handle0x19,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x20],(unsigned int)asm_handle0x20,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x21],(unsigned int)asm_handle0x21,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x22],(unsigned int)asm_handle0x22,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x23],(unsigned int)asm_handle0x23,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x24],(unsigned int)asm_handle0x24,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x25],(unsigned int)asm_handle0x25,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x26],(unsigned int)asm_handle0x26,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x27],(unsigned int)asm_handle0x27,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x28],(unsigned int)asm_handle0x28,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x29],(unsigned int)asm_handle0x29,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x2a],(unsigned int)asm_handle0x2a,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x2b],(unsigned int)asm_handle0x2b,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x2c],(unsigned int)asm_handle0x2c,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x2d],(unsigned int)asm_handle0x2d,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x2e],(unsigned int)asm_handle0x2e,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x2f],(unsigned int)asm_handle0x2f,CODE_SELECTOR,INTERRUPT_DESCRIP);
	set_idt(&idt_addr[0x40],(unsigned int)asm_send_recv,CODE_SELECTOR,INTERRUPT_DESCRIP|SYS_CALL_DESCRIP);
	lidt(IDT_LENGTH,(unsigned int)idt_addr);
	free_memory_add=(((unsigned int)free_memory_add)+IDT_SIZE*sizeof(struct idt));
}
void set_idt(struct idt *i,unsigned int offset,int selector,int ar)
{
	i->offset_low=offset&0xffff;
	i->selector=selector&0xffff;
	i->nothing=0;
	i->ar=ar&0xff;
	i->offset_high=(offset>>16)&0xffff;
}
void set_gdt(struct gdt *g,unsigned int base,unsigned limit,int ar)
{
	if(limit>0xfffff)
	{
		ar=ar|BIG_DESCRIP;
		limit=limit/0x1000;
	}
	g->limit_low=limit&0xffff;
	g->base_low=base&0xffff;
	g->base_mid=(base>>16)&0xff;
	g->base_high=(base>>24)&0xff;
	g->ar=ar&0xff;
	g->limit_high=(ar>>8)&0xf0|(limit>>16)&0xf;
}

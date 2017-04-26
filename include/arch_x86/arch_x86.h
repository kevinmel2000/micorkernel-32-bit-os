#ifndef ARCH_X86_H
#define ARCH_X86_H
//  defines about the intel x86 things
#define CODE_SELECTOR   0x10
#define INTERRUPT_DESCRIP 0x8e
#define SYS_CALL_DESCRIP 0X60
#define GDT_ADD         0x100000
#define IDT_ADD         0x110000
#define GDT_LENGTH      0xffff
#define IDT_LENGTH      0x7ff
#define GDT_SIZE        8192
#define IDT_SIZE        256
#define CODE_DATA_DESCRIP 0x10
#define SEGMENT_PRESENT 0x80  
#define CODE_DESCRIP    0xa
#define DATA_DESCRIP    0x2
#define BIG_DESCRIP     0x8000
#define DEFAULT_DESCRIP 0x4000
#define X86_MAX_SIZE  0xffffffff
#define KERNEL_SELECTOR 24
#define KERNEL_EFLAG    0x1202
#define USEFUL_HIGH_ADD 0x200000
// GDT LDT IDT struct define it's from Intel x86 
typedef struct gdt
{
	unsigned short limit_low,base_low;
	unsigned char base_mid,ar,limit_high,base_high;
}ldt;
/*struct ldt
{
	unsigned short limit_low,base_low;
	unsigned char base_mid,ar,limit_high,base_high;
	};*/
struct idt
{
	unsigned short offset_low,selector;
	unsigned char nothing,ar;
	unsigned short offset_high;
};
struct gdt *gdt_addr;
struct idt *idt_addr;
extern void hlt();
extern void sti();
extern void cli();
extern void lgdt(int limit,int base);
extern void lidt(int limit,int base);
extern int in(int port);
extern void out(int port,int msg);
extern void set_gdt(struct gdt *g,unsigned int base,unsigned limit,int ar);
extern void set_idt(struct idt *i,unsigned int offset,int selector,int ar);
extern void ltr(int selector);
extern int in_word(int port);
extern void out_word(int port,int message);
extern void out_dword(int port,int msg);
extern int in_dword(int port);
extern void init_gdt_idt();
extern void play();     // temp to debug
extern void playb();    // temp to debug
extern void playc();    // temp to debug
extern void playd();    // temp to debug
extern void reset_stack(unsigned int new_stack_add);   // reset the kernel stack
// reset the kernel selector change the GDT selector to the LDT selector
// it's almost switch the task
extern void reset_selector(unsigned int ldt,unsigned int cs,unsigned int ds);
// save the driver list into memory
extern void init_driver_list();
// the most function to  switch task 
extern void far_jump(unsigned int base,unsigned int selector);
#endif

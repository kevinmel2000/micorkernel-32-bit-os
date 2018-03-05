#ifndef MYOS_H
#define MYOS_H
#define KERNEL_ADD      0x500
#define KERNEL_SIZE_ADD 0x502 
#define DRIVER_NUM      0x504
#define DRIVER_DESC_ADD 0x600
#define FS              0x1
#define HD              0x2
#define TTY             0x4
#define CONSOLE         0x8
#define KEYBOARD        0x10
#define SHELL           0x20
extern void init_gdt_idt(unsigned int kernel_size);
extern void init_task_sys();
extern int strcmp(char *s1,char *s2);
extern void playb();
unsigned int kernel_size;
#endif

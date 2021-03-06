#ifndef INTERUPT_H
#define INTERUPT_H
#define PAGE_PRESENT 1
#define PAGE_READ 0
#define PAGE_WRITE 2
#define PAGE_USER 4
#define PAGE_SUPER 0
#define NULL 0
#define HANDLE_NUM 0x20
#define HANDLE_NUM_EC 10
#define HANDLE_NUM_HARD 0X10
#include <IPC/message.h>   
// normal exception handler list 
struct handle_list
{
  void (*handler[HANDLE_NUM])(int *esp);
};
// error code interrupt handler list 
struct handle_list_ec
{
  void (*handler_ec[HANDLE_NUM_EC])(unsigned int error_ip,unsigned int error_code);
};
// hardware interrupt handler list
struct handle_list_hard
{
  void (*handler_hard[HANDLE_NUM_HARD])(int qir_num);
};
// software interrupt handler 
// the system call interface
struct send_recv_s
{
  void (*send_recv)(void *msg);//(struct message *msg);
};
// hardware interrupt request list
struct regist_hard_interrupt
{

};
// hardware interrupt handler
extern void asm_handle0x20();
extern void asm_handle0x21();
extern void asm_handle0x22();
extern void asm_handle0x23();
extern void asm_handle0x24();
extern void asm_handle0x25();
extern void asm_handle0x26();
extern void asm_handle0x27();
extern void asm_handle0x28();
extern void asm_handle0x29();
extern void asm_handle0x2a();
extern void asm_handle0x2b();
extern void asm_handle0x2c();
extern void asm_handle0x2d();
extern void asm_handle0x2e();
extern void asm_handle0x2f();

// software exception handler
extern void asm_handle0x00();
extern void asm_handle0x05();
extern void asm_handle0x07();
extern void asm_handle0x08();
extern void asm_handle0x09();
extern void asm_handle0x10();
extern void asm_handle0x11();
extern void asm_handle0x12();
extern void asm_handle0x13();
extern void asm_handle0x14();
extern void asm_handle0x16();
extern void asm_handle0x17();
extern void asm_handle0x18();
extern void asm_handle0x19();
//extern void asm_handle0x2e();
extern void handle0x00(int *esp);
extern void handle0x05(int *esp);
extern void handle0x07(int *esp);
extern void handle0x08(unsigned int error_ip,unsigned int error_code);
extern void handle0x10(unsigned int error_ip,unsigned int error_code);
extern void handle0x11(unsigned int error_ip,unsigned int error_code);
extern void handle0x12(unsigned int error_ip,unsigned int error_code);
extern void handle0x13(unsigned int error_ip,unsigned int error_code);
extern void handle0x14(unsigned int error_ip,unsigned int error_code);
extern void handle0x17(unsigned int error_ip,unsigned int error_code);
extern void handle0x09(int *esp);
extern void handle0x16(int *esp);
extern void handle0x18(int *esp);
extern void handle0x19(int *esp);
// hardware handler
extern void handle0x20();
extern void handle0x21();
extern void handle0x22();
extern void handle0x23();
extern void handle0x24();
extern void handle0x25();
extern void handle0x26();
extern void handle0x27();
extern void handle0x28();
extern void handle0x29();
extern void handle0x2a();
extern void handle0x2b();
extern void handle0x2c();
extern void handle0x2d();
extern void handle0x2e();
extern void handle0x2f();


extern void dft_handler(int *esp);
extern void dft_handler_ec(unsigned int error_ip,unsigned int error_code);
extern void dft_handler_hard(int irq_num);
// default hardware interrupt handler
extern void dft_send_recv(void *msg);

// the asm code call this functon
extern void sys_send_recv(void *msg);

// this handler write into the IDT in intel
extern void asm_send_recv();

// initialize the default interrupt handler
extern void init_dft_interupt();

// set handler to the specified interrupt number
extern void set_handler(unsigned int num,void *function);

struct handle_list *h_list;
struct handle_list_ec *h_list_ec;
struct handle_list_hard *h_list_hard;
struct send_recv_s *h_list_msg;
#endif 

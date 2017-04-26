#include <arch_x86/interrupt/interrupt.h>
#include <memory.h>
#include <arch_x86/chipset.h> 
#include <task.h>
void init_dft_interupt(int *esp)
{
  int i=0;
  h_list=(struct handle_list*)alloc_memory(KERNEL_SPACE,sizeof(struct handle_list));
  for(i=0;i<HANDLE_NUM;i++)
    {
      h_list->handler[i]=dft_handler;
    }
  h_list_ec=(struct handle_list_ec*)((unsigned int)h_list+4*HANDLE_NUM);
  for(i=0;i<HANDLE_NUM_EC;i++)
    {
      h_list_ec->handler_ec[i]=dft_handler_ec;
    }

  // get a place for the h_list_msg 
  h_list_msg=(struct send_recv_s*)((unsigned int)h_list_ec+4*HANDLE_NUM_EC);
  // set default send_recv handler with do nothing
  h_list_msg->send_recv=dft_send_recv;

  // hard handler list
  h_list_hard=(struct handle_list_hard*)((unsigned int)h_list_msg+4);
  // set hard handler function
  for(i=0;i<HANDLE_NUM_HARD;i++)
    {
      h_list_hard->handler_hard[i]=dft_handler_hard;
    }
}
void dft_handler(int *esp)
{
  // while(1);
  //playd();
  return;
}
void dft_handler_ec(unsigned int error_ip,unsigned int error_code)
{
  // while(1);
  //playd();
  return;
}
void  dft_send_recv(void *msg)
{
  return;
}
void dft_handler_hard(int irq_num)
{
  /**
   *  send message to process,which waitting for this hard handler 
   *
   *
   *
   *
   */
  return;
}
void set_handler(unsigned int num,void *function)
{
  if(num==0x40)
    {
      h_list_msg->send_recv=function;
      return;
    }
  if(num==8)
    {
      h_list_ec->handler_ec[0]=function;
      return;
    }
  if(num<0x20&&num!=8&&num!=5&&num!=7&&num!=8&&num!=10&&num!=11&&num!=11&&num!=12&&num!=13&&num!=14&&num!=17)
    {
      h_list->handler[num]=function;
    }
  else if(num>=0x20&&num<0x30)
    {
      h_list_hard->handler_hard[num]=function;
    }
}
void handle0x00(int *esp)
{
  //playd();
  h_list->handler[0](esp);
}
void handle0x05(int *esp)
{
  //playd();
  h_list->handler[5](esp);
}
void handle0x07(int *esp)
{
  h_list->handler[7](esp);
}
void handle0x08(unsigned int error_ip,unsigned int error_code)
{
  h_list_ec->handler_ec[0](error_ip,error_code);
}
void handle0x09(int *esp)
{
  h_list->handler[9](esp);
}
void handle0x10(unsigned int error_ip,unsigned int error_code)
{
  // playd();
  h_list_ec->handler_ec[1](error_ip,error_code);
}
void handle0x11(unsigned int error_ip,unsigned int error_code)
{
  //playd();
  h_list_ec->handler_ec[2](error_ip,error_code);
}
void handle0x12(unsigned int error_ip,unsigned int error_code)
{
  //playd();
  h_list_ec->handler_ec[3](error_ip,error_code);
}
void handle0x13(unsigned int error_ip,unsigned int error_code)
{
  playd();
  h_list_ec->handler_ec[4](error_ip,error_code);
}
void handle0x14(unsigned int error_ip,unsigned int error_code)
{
  // playd();
    h_list_ec->handler_ec[5](error_ip,error_code);
}
void handle0x16(int *esp)
{
  h_list->handler[16](esp);
}
void handle0x17(unsigned int error_ip,unsigned int error_code)
{
  //playd();
  h_list_ec->handler_ec[6](error_ip,error_code);
}
void handle0x18(int *esp)
{
  h_list->handler[18](esp);
}
void handle0x19(int *esp)
{
  h_list->handler[19](esp);
}
void handle0x20()
{
  // playb();
  out(0x20,0x20);
  //no_page_nf();
  //no_page_trans_add_nf();
  h_list_hard->handler_hard[0](0);
  //h_list->handler[0x20](esp);
  //set_page_trans_add_nf();
  //set_page_nf();
}
void handle0x21()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[1](1);
  //h_list->handler[0x21](esp);
}
void handle0x22()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[2](2);
  //h_list->handler[0x22](esp);
}
void handle0x23()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[3](3);
  //h_list->handler[0x23](esp);
}
void handle0x24()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x24](esp);
}
void handle0x25()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x25](esp);
}
void handle0x26()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x26](esp);
}
void handle0x27()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x27](esp);
}
void handle0x28()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x28](esp);
}
void handle0x29()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x29](esp);
}
void handle0x2a()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x2a](esp);
}
void handle0x2b()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x2b](esp);
}
void handle0x2c()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x2c](esp);
}
void handle0x2d()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x2d](esp);
}
void handle0x2e()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x2e](esp);
}
void handle0x2f()
{
  out(0x20,0x20);
  h_list_hard->handler_hard[4](4);
  //h_list->handler[0x2f](esp);
}
void sys_send_recv(void *msg)
{
  h_list_msg->send_recv(msg);
}

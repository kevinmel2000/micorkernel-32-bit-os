#include <kernel.h>
#include <memory.h>
#include <task.h>
#include <arch_x86/interrupt/interrupt.h>
#include <arch_x86/chipset.h>
#include <process_manage/process_manage.h>
#include <IPC/message.h>
#include <string.h>
#include <init.h>
void delay(unsigned int time);
// the main function of this OS , 
// when we finish intialize the pm mm and init process then ,
// this function is useless just run when there is no process can run
void main()
{
  // dissable the interrupt
  cli();
  free_memory_add_high=USEFUL_HIGH_ADD+(kernel_size<<4);
  kernel_size=get_memory(KERNEL_SIZE_ADD)<<4; 
  init_gdt_idt(kernel_size);
  memory_size=check_memory();
  init_memory(memory_size,free_memory_add,free_memory_add_high);
  set_page(memory_size);
  reset_tasks();
  set_kernel_task();
  set_driver_list();
  init_pic();
  init_dft_interupt();
  set_clock();
  set_handler(0x8,page_handler);
  play();
  set_pm_task();
  set_mm_task();
  init_message();
  set_init_task();
  //while(1);
  // enable the interrupt
  sti();
  while(1)
    {
      struct message msg;
      msg.type=ROUND_TRIP;
      msg.dst=1;
      msg.function=FORK;
      playb();
      //int40(&msg);
      while(1)
      {
	delay(1000);
	playc();
      }
      if(msg.function==0)
	{
	  while(1)
	    {
	      delay(100);
	      playc();
	    }
	}
      else
	{
	  while(1)
	    {
	      delay(100);
	      playb();
	    }
	  } 
    }
}
void delay(unsigned int time)
{
  int i,j;
  for(i=0;i<time;i++)
    {
      for(j=0;j<time;j++)
	{
	}
    }
}

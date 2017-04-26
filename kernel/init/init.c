#include <memory.h>
#include <task.h>
#include <kernel.h>
#include <process_manage/process_manage.h>
#include <memory_manage/memory_manage.h>
#include <signal.h>

void temp()
{
  int a=0;
}

void init()
{
  struct message msg;
  msg.type=ROUND_TRIP;
  msg.function=REDEF_SIGNAL;
  msg.dst=1;
  msg.stright_msg=(unsigned int)temp;
  msg.return_value=1;
  int40(&msg);
  msg.type=SEND;
  msg.function=SIGNAL;
  msg.dst=1;
  msg.stright_msg=2;
  int40(&msg);
  while(1);
}
void set_init_task()
{
  int i=0,j=0;
  int page_number=memory_size/0x400000;
  unsigned int *kernel_pde=(unsigned int*)kernel_page_add;
  unsigned int *kernel_pte=NULL;
  unsigned int init_add=alloc_memory(KERNEL_SPACE,2*SIZE_OF_PAGE);
  unsigned int stack_add=alloc_memory(USER_SPACE,2*SIZE_OF_PAGE);
  unsigned int *pde_add=(unsigned int*)(init_add+SIZE_OF_PAGE);
  unsigned int *pte_add=(unsigned int*)init_add;
  pde_add[0]=kernel_pde[0]|PAGE_USER;
  pde_add[1]=kernel_pde[1]|PAGE_USER;
  for(i=0;i<1024;i++)
    {
      pte_add[i]=NULL;
    }
  pde_add[2]=((unsigned int)pte_add)|PAGE_PRESENT|PAGE_USER|PAGE_WRITE;
  for(i=3;i<page_number;i++)
    {
      pde_add[i]=kernel_pde[i];
    }
  struct task *init_task=(struct task*)((unsigned int)pde_add+4*page_number);
  copy_memory((char*)t_list->task_struct_add[0]->this_ldt,(char*)init_task->this_ldt,LDT_SIZE*sizeof(ldt));
  init_task->this_ldt[DS_LDT].ar=init_task->this_ldt[DS_LDT].ar|(USER_AR<<5);
  init_task->this_ldt[CS_LDT].ar=init_task->this_ldt[CS_LDT].ar|(USER_AR<<5);
  init_task->tasc.cr3=(unsigned int)pde_add;
  init_task->tasc.eip=(unsigned int)init;
  init_task->tasc.eflags=KERNEL_EFLAG;
  init_task->tasc.eax=\
  init_task->tasc.ecx=\
  init_task->tasc.edx=\
  init_task->tasc.ebx=\
  init_task->tasc.edi=\
  init_task->tasc.edi=0;
  init_task->tasc.ss0=DS_GDT<<3;
  init_task->tasc.es=\
  init_task->tasc.ds=\
  init_task->tasc.fs=\
  init_task->tasc.gs=\
  init_task->tasc.ss=(DS_LDT<<3)|LDT_SELECTOR|USER_AR;
  init_task->tasc.cs=(CS_LDT<<3)|LDT_SELECTOR|USER_AR;
  init_task->tasc.iomap_base=NORMAL_IOMAPBASE;
  init_task->status=RUNNING;
  init_task->time=2;
  init_task->entry=(unsigned int)init;
  unsigned int init_task_gdt=find_empty_gdt();
  unsigned int init_task_pid=find_empty_process();
  t_list->task_struct_add[init_task_pid]=init_task;
  init_task->user_esp=alloc_page(init_task_pid,stack_add);
  init_task->tasc.esp=init_task->user_esp+SIZE_OF_PAGE;
  init_task->tasc.esp0=alloc_page(init_task_pid,stack_add+SIZE_OF_PAGE)+SIZE_OF_PAGE;
  init_task->tasc.ldt=(init_task_gdt+1)<<3;
  init_signal_handler((struct signal_handle*)init_task->signal_handler);
  init_task->signal_mask=0xffffffff;
  set_gdt(&gdt_addr[init_task_gdt],(unsigned int)&init_task->tasc,TSS_SIZE,TSS_DESCRIB);
  set_gdt(&gdt_addr[init_task_gdt+1],(unsigned int)init_task->this_ldt,LDT_SIZE*sizeof(ldt)-1,LDT_DESCRIB);
  init_task->selector=init_task_gdt<<3;
  init_task->father=-1;
  init_task->child=-1;
  init_task->brother=-1;
}

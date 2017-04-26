#include <process_manage/process_manage.h>
#include <task.h>
#include <memory.h>
#include <kernel.h>
#include <IPC/message.h>
#include <signal.h>

void process_manage()
{
  struct signal_wait_list *sw_list=(struct signal_wait_list*)alloc_memory(USER_SPACE,SIZE_OF_PAGE);
  init_signal_wait_list(sw_list);
  int new_pid=-1;
  while(1)
    {
      struct message msg;
      msg.src=ANY;
      receive_message(&msg);

      switch(msg.function)
	{
	case SIGNAL:
	  do_signal(msg.src,msg.stright_msg);
	  break;
	case REDEF_SIGNAL:
	  redefine_handler(msg.src,msg.return_value,(void*)msg.stright_msg);
	  msg.dst=msg.src;
	  send_message(&msg);
	  break;
	case FORK:
	  cli();
	  new_pid=do_fork(msg.src);
	  sti();
	  msg.dst=msg.src;
	  msg.return_value=new_pid;
	  send_message(&msg);
	  msg.dst=new_pid;
	  msg.return_value=0;
	  send_message(&msg);
	  break;
	case EXIT:
	  cli();
	  do_exit(msg.src,sw_list);
	  sti();
	  break;
	case 0:
	  break;
	case 9999:
	  break;
	}
    }
}

void set_pm_task()
{
  unsigned int pm_add=alloc_memory(KERNEL_SPACE,SIZE_OF_PAGE);
  unsigned int pde_add=kernel_page_add;
  struct task *pm_task=(struct task*)pm_add;
  copy_memory((char*)t_list->task_struct_add[0]->this_ldt,(char*)pm_task->this_ldt,LDT_SIZE*sizeof(ldt));
  pm_task->tasc.cr3=kernel_page_add;
  pm_task->tasc.eip=(unsigned int)process_manage;
  pm_task->tasc.eflags=KERNEL_EFLAG;
  pm_task->tasc.eax=\
  pm_task->tasc.ecx=\
  pm_task->tasc.edx=\
  pm_task->tasc.ebx=\
  pm_task->tasc.edi=\
  pm_task->tasc.edi=0;
  pm_task->tasc.esp=pm_task->tasc.ebp=pm_add+SIZE_OF_PAGE;
  pm_task->tasc.es=\
  pm_task->tasc.ds=\
  pm_task->tasc.fs=\
  pm_task->tasc.gs=\
  pm_task->tasc.ss=(DS_LDT<<3)|LDT_SELECTOR;
  pm_task->tasc.cs=(CS_LDT<<3)|LDT_SELECTOR;
  pm_task->tasc.iomap_base=NORMAL_IOMAPBASE;
  pm_task->status=RUNNING;
  pm_task->time=2;
  pm_task->entry=(unsigned int)process_manage;
  unsigned int pm_task_gdt=find_empty_gdt();
  unsigned int pm_task_pid=find_empty_process();
  pm_task->tasc.ldt=(pm_task_gdt+1)<<3;
  set_gdt(&gdt_addr[pm_task_gdt],(unsigned int)&pm_task->tasc,TSS_SIZE,TSS_DESCRIB);
  set_gdt(&gdt_addr[pm_task_gdt+1],(unsigned int)pm_task->this_ldt,LDT_SIZE*sizeof(ldt)-1,LDT_DESCRIB);
  pm_task->selector=pm_task_gdt<<3;
  pm_task->proce_memory.not_allocted=NULL;
  pm_task->father=-1;
  pm_task->child=-1;
  pm_task->brother=-1;
  t_list->task_struct_add[pm_task_pid]=pm_task;
}

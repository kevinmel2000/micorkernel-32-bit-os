#include <myos.h>
#include <memory_manage/memory_manage.h>
#include <task.h>
#include <memory.h>
#include <kernel.h>
#include <IPC/message.h>

void memory_manage()
{
  remap_l=(struct remap_list*)alloc_memory(USER_SPACE,SIZE_OF_PAGE);
  init_remap_list(remap_l);
  while(1)
    {
      //
      // prepare the message to receive from other process
      //
      struct message msg;
      msg.type=RECEIVE;
      msg.src=ANY;
      int40(&msg);
      if(msg.function==REMAP)
	{
	  unsigned int line=0;
	  if(msg.return_value!=0)
	    {
	      line=msg.return_value;
	      // remap the page address of the physicle address
	      // param is pid,line address,physicle address
	      remap_page((unsigned int*)t_list->task_struct_add[msg.src]->tasc.cr3,msg.return_value,msg.stright_msg);
	    }
	  else
	    {
	      // alloc a page into the page table
	      // param is pid and physicle address
	      line=alloc_page(msg.src,msg.stright_msg);
	      msg.return_value=line;
	    }
	  // insert remap info into the remap list
	  if(!insert_remap_list(msg.src,remap_l,line))
	    {
	      msg.return_value=0;
	    }
	  msg.type=SEND;
	  msg.dst=msg.src;
	  int40(&msg);
	}
      if(msg.function==MALLOC)
	{
	  void *temp=sys_malloc(msg.src,msg.stright_msg);
	  msg.return_value=(unsigned int)temp;
	  msg.dst=msg.src;
	  msg.type=SEND;
	  int40(&msg);
	}
      if(msg.function==FREE)
	{
	  free_remap(msg.src,remap_l,msg.stright_msg);
	  //
	  //will insert free alloc memory code here
	  //
	  //
	}
    }
}
void set_mm_task()
{
  unsigned int mm_add=alloc_memory(KERNEL_SPACE,SIZE_OF_PAGE);
  unsigned int pde_add=kernel_page_add;
  struct task *mm_task=(struct task*)mm_add;
  copy_memory((char*)t_list->task_struct_add[0]->this_ldt,(char*)mm_task->this_ldt,LDT_SIZE*sizeof(ldt));
  mm_task->tasc.cr3=kernel_page_add;
  mm_task->tasc.eip=(unsigned int)memory_manage;
  mm_task->tasc.eflags=KERNEL_EFLAG;
  mm_task->tasc.eax=\
  mm_task->tasc.ecx=\
  mm_task->tasc.edx=\
  mm_task->tasc.ebx=\
  mm_task->tasc.edi=\
  mm_task->tasc.edi=0;
  mm_task->tasc.esp=mm_task->tasc.ebp=mm_add+SIZE_OF_PAGE;
  mm_task->tasc.es=\
  mm_task->tasc.ds=\
  mm_task->tasc.fs=\
  mm_task->tasc.gs=\
  mm_task->tasc.ss=(DS_LDT<<3)|LDT_SELECTOR;
  mm_task->tasc.cs=(CS_LDT<<3)|LDT_SELECTOR;
  mm_task->tasc.iomap_base=NORMAL_IOMAPBASE;
  mm_task->status=RUNNING;
  mm_task->time=2;
  mm_task->entry=(unsigned int)memory_manage;
  unsigned int mm_task_gdt=find_empty_gdt();
  unsigned int mm_task_pid=find_empty_process();
  mm_task->tasc.ldt=(mm_task_gdt+1)<<3;
  set_gdt(&gdt_addr[mm_task_gdt],(unsigned int)&mm_task->tasc,TSS_SIZE,TSS_DESCRIB);
  set_gdt(&gdt_addr[mm_task_gdt+1],(unsigned int)mm_task->this_ldt,LDT_SIZE*sizeof(ldt)-1,LDT_DESCRIB);
  mm_task->selector=mm_task_gdt<<3;
  mm_task->proce_memory.not_allocted=NULL;
  mm_task->father=-1;
  mm_task->child=-1;
  mm_task->brother=-1;
  t_list->task_struct_add[mm_task_pid]=mm_task;
}

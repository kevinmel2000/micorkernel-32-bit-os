#include <task.h>
#include <kernel.h>

// initialize the pro list 
void reset_tasks()
{
  int i;
  t_list=(struct task_manager*)alloc_memory(KERNEL_SPACE,sizeof(struct task_manager));
  t_list->task_number=0;
  for(i=0;i<MAX_TASK_NUMBER;i++)
    {
      t_list->task_struct_add[i]=NULL;
    }
}
void set_kernel_task()
{
  struct task *kernel_task=(struct task*)alloc_memory(KERNEL_SPACE,sizeof(struct task)+SIZE_OF_PAGE);
  reset_stack((unsigned int)kernel_task+2*SIZE_OF_PAGE);
  copy_memory((char*)&gdt_addr[DS_GDT],(char*)&kernel_task->this_ldt[DS_LDT],sizeof(struct gdt));
  copy_memory((char*)&gdt_addr[CS_GDT],(char*)&kernel_task->this_ldt[CS_LDT],sizeof(struct gdt));
  kernel_task->selector=KERNEL_SELECTOR;
  kernel_task->time=2;
  kernel_task->status=RUNNING;
  kernel_task->next=0;
  kernel_task->tasc.iomap_base=NORMAL_IOMAPBASE;
  kernel_task->tasc.eflags=KERNEL_EFLAG;
  kernel_task->tasc.cr3=kernel_page_add;
  kernel_task->tasc.cs=(CS_LDT<<3)|LDT_SELECTOR;
  kernel_task->tasc.ds=(DS_LDT<<3)|LDT_SELECTOR;
  kernel_task->tasc.es=(DS_LDT<<3)|LDT_SELECTOR;
  kernel_task->tasc.gs=(DS_LDT<<3)|LDT_SELECTOR;
  kernel_task->tasc.ss=(DS_LDT<<3)|LDT_SELECTOR;
  kernel_task->tasc.fs=(DS_LDT<<3)|LDT_SELECTOR;
  kernel_task->tasc.ldt=4<<3;
  kernel_task->father=-1;
  kernel_task->child=-1;
  kernel_task->brother=-1;
  set_gdt(&gdt_addr[3],(unsigned int)&kernel_task->tasc,TSS_SIZE,TSS_DESCRIB);
  set_gdt(&gdt_addr[4],(unsigned int)kernel_task->this_ldt,LDT_SIZE*sizeof(ldt)-1,LDT_DESCRIB);
  kernel_task->proce_memory.not_allocted=NULL;
  t_list->task_number++;
  t_list->task_struct_add[0]=kernel_task;
  t_list->now_runing=0;
  t_list->counter=kernel_task->time;
  ltr(3*8);
  reset_selectors(kernel_task->tasc.ldt,kernel_task->tasc.cs&0xffff,kernel_task->tasc.ds&0xffff);
}

// set the driver list from the driver memory sapce
void set_driver_list()
{
  int i;
  driver_list=(struct driver_load_manage*)alloc_memory(KERNEL_SPACE,sizeof(struct driver_load_manage));
  unsigned int driver_num=get_memory(DRIVER_NUM)&0xffff;
  driver_list->driver_num=driver_num;
  for(i=0;i<driver_num;i++)
    {
      driver_list->drivers[i].type=get_memory(DRIVER_DESC_ADD+i*4)&0xffff;
      driver_list->drivers[i].add=(void*)((get_memory(DRIVER_DESC_ADD+i*4+2)&0xffff)<<4);
    }
}

// switch task to a runnable process
void switchtask()
{
  unsigned int pre_task=t_list->now_runing;
  unsigned int i=pre_task+1;
  while(1)
    {
      if(t_list->task_struct_add[i]!=NULL&&t_list->task_struct_add[i]->status==RUNNING)
	{
	  break;
	}
      i++;
      if(i>=MAX_TASK_NUMBER)
	{
	  i=0;
	}
    }
  t_list->now_runing=i;
  t_list->counter=t_list->task_struct_add[i]->time;
  if(i!=pre_task)
    {
      far_jump(0,t_list->task_struct_add[i]->selector);
    }
}

// find a empty place from the process list
unsigned int find_empty_process()
{
  unsigned int i=0;
  for(i=0;i<MAX_TASK_NUMBER;i++)
    {
      if(t_list->task_struct_add[i]==NULL)
	{
	  return i;
	}
    }
  return 0; 
}

// release a place from the task table
void free_process(unsigned int pid)
{
  if(pid>=MAX_TASK_NUMBER)
    {
      return;
    }
  t_list->task_struct_add[pid]=NULL;
}

// find a empty place from GDT table
unsigned int find_empty_gdt()
{
  unsigned int i;
  for(i=1;i<GDT_SIZE;i++)
    {
      if(gdt_addr[i].ar==NULL&&gdt_addr[i+1].ar==NULL)
	{
	  return i;
	}
    }
  return 0;
}

// release the place from the get table
void free_gdt(unsigned int gdt)
{
  if(gdt>=GDT_SIZE)
    {
      return;
    }
  gdt_addr[gdt].ar=NULL;
}

// lock the specifiec pro
void lock(unsigned int pid)
{
  //insert to check task 0
  if(t_list->task_struct_add[pid]==NULL)
    {
      return;
    }
  t_list->task_struct_add[pid]->status=HANG_UP;
  switchtask();
}

// unlock the speciefic pro
void unlock(unsigned int pid)
{
  // insert to check task 0
  if(t_list->task_struct_add[pid]==NULL)
    {
      return;
    }
  t_list->task_struct_add[pid]->status=RUNNING;
}


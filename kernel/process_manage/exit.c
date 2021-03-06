#include <kernel.h>
#include <memory.h>
#include <task.h>
#include <process_manage/process_manage.h>
#include <memory_manage/memory_manage.h>
void do_exit(int pid,struct signal_wait_list *l)
{
  if(pid<=0)
  {
   return;
  }
  t_list->task_struct_add[pid]->status=HANG_UP;
  free_process_memory(pid);
  free_gdt(t_list->task_struct_add[pid]->tasc.ldt>>3);
  free_gdt(t_list->task_struct_add[pid]->selector>>3);
  clear_message(pid);
  clear_signal_wait_list(pid,l);
  t_list->task_struct_add[pid]=NULL;
}
void free_process_memory(int pid)
{
  int i=0;
  unsigned int *pde_add=(unsigned int*)t_list->task_struct_add[pid]->tasc.cr3;
  unsigned int *pte_add=(unsigned int*)(pde_add[2]&0xfffff000);
  // release the remap page in this process page table
  free_remap_all(pid,remap_l);
  // the rest of page in page table all allocted a physicle memory 

  // now release the physicle memory this process used
  for(i=0;i<1024;i++)
    {
      unsigned int physicle_add=pte_add[i];
      if(physicle_add&PAGE_WRITE)
	{
	  free_memory(physicle_add&0xfffff000,SIZE_OF_PAGE);
	}
    }

  // release this process PCB of  kernel space
  free_memory((unsigned int)pde_add,2*SIZE_OF_PAGE);
}

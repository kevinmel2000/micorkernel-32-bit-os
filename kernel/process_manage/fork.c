#include <process_manage/process_manage.h>
#include <task.h>
#include <memory.h>
int do_fork(unsigned int father)
{
  // cli();
  int i;
  int j;
  int page_number=memory_size/0x400000;
  struct task *parent=t_list->task_struct_add[father];
  unsigned int *parent_pde=(unsigned int*)parent->tasc.cr3;
  unsigned int *parent_pte=NULL;
  unsigned int *pte_add=(unsigned int*)alloc_memory(KERNEL_SPACE,2*SIZE_OF_PAGE);
  unsigned int stack_space=alloc_memory(USER_SPACE,2*SIZE_OF_PAGE);
  if(pte_add==NULL)
    {
      return -1;
    }
  unsigned int *pde_add=(unsigned int*)(((unsigned int)pte_add)+SIZE_OF_PAGE);
  struct task *child=(struct task*)(((unsigned int)pte_add)+SIZE_OF_PAGE+4*page_number);
  copy_memory((char*)parent,(char*)child,sizeof(struct task));
  child->tasc.cr3=(unsigned int)pde_add;
  for(i=0;i<page_number;i++)
    {
      pde_add[i]=parent_pde[i];
    }
  parent_pte=(unsigned int*)(parent_pde[2]&0xfffff000);
  for(i=0;i<1024;i++)
    {
      pte_add[i]=parent_pte[i]=parent_pte[i]&(~PAGE_WRITE);
    }
  pde_add[2]=(unsigned int)pte_add|PAGE_USER|PAGE_PRESENT;
  unsigned int parent_stack_add=line2physicle(father,parent->tasc.esp);
  unsigned int parent_user_stack_add=line2physicle(father,parent->user_esp);
  copy_memory((char*)parent_user_stack_add,(char*)stack_space,SIZE_OF_PAGE);
  copy_memory((char*)(parent_stack_add&0xfffff000),(char*)(stack_space+SIZE_OF_PAGE),SIZE_OF_PAGE);
  // remap_page((unsigned int*)parent->tasc.cr3,parent->tasc.esp,parent->tasc.esp&0xfffff000);
  set_page_use((unsigned int*)parent->tasc.cr3,parent->tasc.esp);
  remap_page((unsigned int*)child->tasc.cr3,child->tasc.esp,stack_space+SIZE_OF_PAGE);
  // remap_page((unsigned int*)parent->tasc.cr3,parent->user_esp,parent_user_stack_add);
  set_page_use((unsigned int*)parent->tasc.cr3,parent->user_esp);
  remap_page((unsigned int*)child->tasc.cr3,child->user_esp,stack_space);
  unsigned int new_pid=find_empty_process();
  if(new_pid==0)
    {
      free_memory((unsigned int)pte_add,2*SIZE_OF_PAGE);
      return -1;
    }
  unsigned int new_gdt=find_empty_gdt();
  if(new_gdt==0)
    {
      free_memory((unsigned int)pte_add,2*SIZE_OF_PAGE);
      free_process(new_pid);
      return -1;
    }
  set_gdt(&gdt_addr[new_gdt],(unsigned int)&child->tasc,TSS_SIZE,TSS_DESCRIB);
  set_gdt(&gdt_addr[new_gdt+1],(unsigned int)child->this_ldt,LDT_SIZE*sizeof(ldt)-1,LDT_DESCRIB);
  child->tasc.ldt=(new_gdt+1)<<3;
  child->selector=new_gdt<<3;
  child->father=father;
  child->brother=-1;
  child->brother=parent->child;
  parent->child=new_pid;
  t_list->task_struct_add[new_pid]=child;
  //sti();
  return new_pid;
}

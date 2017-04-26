#include <memory.h>
#include <task.h>
#include <arch_x86/interrupt/interrupt.h>
#include <kernel.h>
unsigned int *not_allocted_add;
//**************use in vsersion 2015*********************//
void init_memory(unsigned int memory_size,unsigned int free_memory_add,unsigned int free_memory_add_high)
{
  int i;
  if((USEFUL_HIGH_ADD-free_memory_add)>sizeof(struct memory_manage))
    {
      memory_list=(struct memory_manage*)((free_memory_add&0xfffffff0)+0x10);
    }
  else
    {
      while(1);
    }
  for(i =0;i<ROOM_SIZE;i++)
    {
      memory_list->room[i].base=memory_list->room[i].size=0;
    }
  memory_list->base=0;
  free_memory_add+=sizeof(struct memory_manage);
  memory_list->base=(unsigned int)free_memory_add_high;
  memory_list->room[0].base=(free_memory_add&0xfffff000)+SIZE_OF_PAGE;
  memory_list->room[0].size=USEFUL_HIGH_ADD-memory_list->room[0].base;
  unsigned int kernel_used=(get_memory(KERNEL_SIZE_ADD)&0xffff)<<4;
  kernel_used=(kernel_used&0xfffff000)+SIZE_OF_PAGE*2;
  memory_list->room[1].base=((unsigned int)free_memory_add_high)+kernel_used;
  memory_list->room[1].size=(memory_size>>2)-memory_list->room[1].base;
  memory_list->room[2].base=memory_size>>2;
  memory_list->room[2].size=memory_size-memory_list->room[2].base;
}
//************************************************************//
//*************use in version 2015************************//
void set_page(unsigned int memory_size)
{
  unsigned int size_of_pde=memory_size/(SIZE_OF_PAGE*1024);
  unsigned int size_of_pte=memory_size/SIZE_OF_PAGE;
  unsigned int *pde_add;
  unsigned int *pte_add;
  unsigned int page_add=alloc_memory(KERNEL_SPACE,SIZE_OF_PAGE+4*size_of_pte);
  int i;
  unsigned int temp=0;
  kernel_page_add=page_add;
  pde_add=(unsigned int*)page_add;
  pte_add=(unsigned int*)(page_add+SIZE_OF_PAGE);
  temp=(unsigned int)pte_add;
  for(i=0;i<size_of_pde;i++) 
    {
      pde_add[i]=temp|PAGE_PRESENT|PAGE_WRITE;
      temp+=SIZE_OF_PAGE;
    }
  temp=0;
  for(i=0;i<size_of_pte;i++)
    {
      pte_add[i]=temp|PAGE_PRESENT|PAGE_USER;
      temp+=SIZE_OF_PAGE;
    }
  store_page(kernel_page_add);
}
//***************************************************//
/*int handle_no_page(unsigned int line_add)
{
  unsigned int *pde_add_temp=(unsigned int*)pde_add;
  unsigned int index_of_pde=line_add>>22&0x3ff;
  unsigned int index_of_pte=line_add>>12&0x3ff;
  unsigned int *pte_add=(unsigned int*)pde_add_temp[index_of_pde];
  unsigned int physicle_add=alloc_memory(KERNEL_SPACE,0x1000);
  if(physicle_add==0)
    {
      return 0;
    }
  pte_add[index_of_pte]=physicle_add+0x7;
  return 1;
  }*/
//******************************************************//
//******************** use in version 2015*********************//
unsigned int alloc_page(int pid,unsigned int physicle_add)
{
  struct task *pro=t_list->task_struct_add[pid];
  unsigned int *pde_add=(unsigned int*)pro->tasc.cr3;
  unsigned int *pte_add=(unsigned int*)(pde_add[2]&0xfffff000);
  int i=0;
  for(;i<1024;i++)
    {
      if(!pte_add[i])
	{
	  pte_add[i]=physicle_add|PAGE_WRITE|PAGE_USER|PAGE_PRESENT;
	  return (2<<22)+(i<<12);
	}
    }
  return NULL;
}
//*************************************************************//
//******************* use in version 2015 **********************//
void free_page(int pid,unsigned int line_add)
{
  struct task *pro=t_list->task_struct_add[pid];
  unsigned int index_of_pde=(line_add>>22)&0x3ff;
  unsigned int index_of_pte=(line_add>>12)&0x3ff;
  unsigned int *pde_add=(unsigned int*)pro->tasc.cr3;
  unsigned int *pte_add=(unsigned int*)(pde_add[index_of_pde]&0xfffff000);
  pte_add[index_of_pte]=NULL;
}
//**************************************************************//
//******************use in version 2015********************************//
unsigned int alloc_memory(unsigned int type,unsigned int size)
{
  unsigned int base;
  int i=0;
  if(size&0xfff)
    {
      size=(size&0xfffff000)+0x1000;
    }
  for(;i<ROOM_SIZE;i++)
    {
      if(memory_list->room[i].size>=size)
	{
	  if((type==KERNEL_SPACE&&(memory_list->room[i].base>=(memory_size>>2)))||(type==USER_SPACE&&(memory_list->room[i].base<(memory_size>>2))))
	    {
	      continue;
	    }
	  base=memory_list->room[i].base;
	  memory_list->room[i].base+=size;
	  memory_list->room[i].size-=size;
	  return base;
	}
    }
  return 0;
}
//*********************************************************//
//*******************ues in version 2015*******************//
void free_memory(unsigned int base,unsigned int size)
{
  int i=0;
  for(;i<ROOM_SIZE;i++)
    {
      if(base+size==memory_list->room[i].base)
	{
	  memory_list->room[i].base=base;
	  memory_list->room[i].size+=size;
	  if(i-1>=0&&memory_list->room[i-1].base+memory_list->room[i-1].size==memory_list->room[i].base)
	    {
	      memory_list->room[i-1].size+=memory_list->room[i].size;
	      memory_list->room[i].base=0;
	      memory_list->room[i].size=0;
	    }
	  return;
	}
      else if(memory_list->room[i].base+memory_list->room[i].size==base)
	{
	  memory_list->room[i].size+=size;
	  if(i+1<ROOM_SIZE&&memory_list->room[i].base+memory_list->room[i].size==memory_list->room[i+1].base)
	    {
	      memory_list->room[i].size+=memory_list->room[i+1].size;
	      memory_list->room[i+1].base=0;
	      memory_list->room[i+1].size=0;
	    }
	  return;
	}
    }
  for(i=0;i<ROOM_SIZE;i++)
    {
      if(memory_list->room[i].base==0&&memory_list->room[i].size==0)
	{
	  memory_list->room[i].size=size;
	  memory_list->room[i].base=base;
	  return;
	}
    }
}
//*****************************************************//
//******************use in version 2015****************//
void copy_memory(char *src,char *aim,unsigned int size)
{
  unsigned int i;
  for(i=0;i<size;i++)
    {
      aim[i]=src[i];
    }
}
//******************************************************//
//******************use in version 2015******************//
unsigned int virtul2line(unsigned int pid,unsigned int virtul_add)
{
  struct task *t=t_list->task_struct_add[pid];
  unsigned int base_high=(unsigned int)t->this_ldt[1].base_high&0xffff;
  unsigned int base_mid=(unsigned int)t->this_ldt[1].base_mid&0xff;
  unsigned int base_low=(unsigned int)t->this_ldt[1].base_low&0xff;
  unsigned int base=base_high<<24|base_mid<<16|base_low;
  return base+virtul_add;
}
//*****************************************************//
//********************************************************//
unsigned int line2physicle(unsigned int pid,unsigned int line_add)
{
  struct task *t=t_list->task_struct_add[pid];
  unsigned int *pde_add=(unsigned int*)t->tasc.cr3;
  unsigned int pde_index=(line_add&0xffa00000)>>22;
  unsigned int pte_index=(line_add&0x3ff000)>>12;
  unsigned int add=line_add&0xfff;
  unsigned int *pte_add=(unsigned int*)(pde_add[pde_index]&0xfffff000);
  unsigned int base=pte_add[pte_index]&0xfffff000;
  return base+add; 
}
//*******************************************************//
/*void copy_stack(unsigned int *src_stack,unsigned int *aim_stack,unsigned int size)
{
  unsigned int i;
  size=size/4;
  for(i=0;i<size;i++)
    {
      aim_stack[i]=src_stack[i];
    }
    }*/
//******************use in version 2015***************************//
void page_handler(unsigned int error_code,unsigned int error_eip)
{
  // playd();
  unsigned int error_add=get_error_add();
  set_page_write(error_add);
}
//**********************************************************//
//***********************************************************//
void set_page_write(unsigned int error_add)
{
  struct task *current=t_list->task_struct_add[t_list->now_runing];
  unsigned int *pte_add=NULL;
  unsigned int *pde_add=(unsigned int*)current->tasc.cr3;
  unsigned int index_of_pde=(error_add&0xffc00000)>>22;
  unsigned int index_of_pte=(error_add&0x3ff000)>>12;
  pte_add=(unsigned int*)(pde_add[index_of_pde]&0xfffff000);
  unsigned int temp=alloc_memory(USER_SPACE,SIZE_OF_PAGE);
  unsigned int line_add=alloc_page(t_list->now_runing,temp);
  copy_memory((char*)(error_add&0xfffff000),(char*)line_add,SIZE_OF_PAGE);
  pte_add[index_of_pte]=temp|PAGE_WRITE|PAGE_USER|PAGE_PRESENT;
  pde_add[index_of_pde]=pde_add[index_of_pde]|PAGE_USER|PAGE_WRITE|PAGE_PRESENT;
  REFRESH_TLB();
  free_page(t_list->now_runing,line_add);
}
//*************************************************************//
void set_page_use(unsigned int *pde_add,unsigned int line_add)
{
  unsigned int index_of_pde=(line_add&0xffc00000)>>22;
  unsigned int index_of_pte=(line_add&0x3ff000)>>12;
  unsigned int *pte_add=(unsigned int*)(pde_add[index_of_pde]&0xfffff000);
  pte_add[index_of_pte]=pte_add[index_of_pte]|PAGE_WRITE|PAGE_USER|PAGE_PRESENT;
  pde_add[index_of_pde]=pde_add[index_of_pde]|PAGE_WRITE|PAGE_USER|PAGE_PRESENT;
}
//***************************************************************//
//******************use in version 2015**************************//
void *sys_malloc(int pid,unsigned int size)
{
  struct task *pro=t_list->task_struct_add[pid];
  unsigned int phy_add=0;
  unsigned int line_add=0;
  struct malloc_struct *m=NULL;
  if(pro->proce_memory.not_allocted==NULL)
    {
      m=(struct malloc_struct*)alloc_memory(USER_SPACE,SIZE_OF_PAGE);
      m->size=SIZE_OF_PAGE;
      m->line_add=alloc_page(pid,(unsigned int)m);
      m->next=NULL;
      pro->proce_memory.not_allocted=(void*)m;
    }
  else
    {
      m=(struct malloc_struct*)pro->proce_memory.not_allocted;
    }
  return (void*)alloc(pid,m,size+4);  
  //  add 4 byte cause have to save the size of allocted
}
//***************************************************************//
//***************  use in version 2015 ***********************//
unsigned int alloc(int pid,struct malloc_struct *m,unsigned int size)
{
  struct task *pro=t_list->task_struct_add[pid];
  // unsigned int line=((unsigned int)(pro->proce_memory.not_allocted))&0xfffff000;
  // struct malloc_struct *m=(struct malloc_struct*)phy_add;
  struct malloc_struct *m_next=m->next;
  unsigned int needed=0;
  unsigned int temp_size=0;
  struct malloc_struct *temp_next;
  unsigned int *put_size=NULL;
  //  m is OK, alloc m //
  if(m->size>=size)
    {
      needed=(unsigned int)m->line_add+4;
      put_size=(unsigned int*)m;
      temp_next=m->next;
      temp_size=m->size;
      m=(struct malloc_struct*)((unsigned int)m+size);
      m->size=temp_size-size;
      m->next=temp_next;
      m->line_add+=size;
      pro->proce_memory.not_allocted=(void*)m;
      *put_size=size;
      return needed;
    }
  //////////////////////


  // m is not OK, but it has a next //
  if(m_next!=NULL)
    {
      while(m_next!=NULL)
	{
	  if(m_next->size>size)
	    {
	      needed=(unsigned int)m_next->line_add+4;
	      put_size=(unsigned int*)m_next;
	      temp_next=m_next->next;
	      temp_size=m_next->size;
	      m_next=(struct malloc_struct*)(((unsigned int )m_next)+size);
	      m_next->size=temp_size-size;
	      m_next->next=temp_next;
	      m->next=m_next;
	      put_size=(unsigned int*)needed;
	      *put_size=size;
	      return needed;
	    }
	  else if(m_next->size==size) 
	    {
	      needed=(unsigned int)m_next->line_add+4;
	      put_size=(unsigned int*)m_next;
	      m->next=m_next->next;
	      *put_size=size;
	      return needed;
	    }
	  m=m_next;
	  m_next=m_next->next;
	}
    }
  ///////////////////////////////////

  // m is not OK and had no next  //
  return 0;
  /////////////////////////////////
}
//*****************************************//
//*****USE IN VERSION 2015*****************//
int sys_free(int pid,unsigned int *line_add)
{
  if(line_add==NULL)
    {
      return 0;
    }
  struct task *pro=t_list->task_struct_add[pid];
  //unsigned int line_list=(unsigned int)pro->prce_memory.not_allocted;
  struct malloc_struct *not_list=(struct malloc_struct*)pro->proce_memory.not_allocted;
  unsigned int *phy_add=(unsigned int*)line2physicle(pid,(unsigned int)line_add);
  unsigned int size=*phy_add;
  struct malloc_struct *m_next=not_list->next;
  struct malloc_struct *temp=(struct malloc_struct*)phy_add;
  temp->line_add=(unsigned int)line_add;
  if((unsigned int)phy_add<(unsigned int)not_list)
    {
      if((unsigned int)phy_add+size==(unsigned int)not_list)
	{
	  temp->size+=not_list->size;
	  temp->next=not_list->next;
	}
      else
	{
	  temp->size=size;
	  temp->next=not_list;
	}
      pro->proce_memory.not_allocted=(void*)(phy_add);
      return 1;
    }
  if(m_next==NULL)
    {
      if((unsigned int)phy_add>(unsigned int)not_list)
	{
	  if((unsigned int)not_list+not_list->size==(unsigned int)phy_add)
	    {
	      not_list->size+=size;
	    }
	  else
	    {
	      temp->size=size;
	      temp->next=NULL;
	      not_list->next=temp;
	    }
	  return 1;
	}
      return 0;
    }
  struct malloc_struct *pointer=not_list;
  while(m_next!=NULL)
    {  
      if((unsigned int)phy_add<(unsigned int)m_next)
	{
	  if((unsigned int)phy_add>(unsigned int)pointer)
	    {
	      temp->size=size;
	      temp->next=m_next;
	      pointer->next=temp;
	      if((unsigned int)temp+size==(unsigned int)m_next)
		{
		  temp->size+=m_next->size;
		  temp->next=m_next->next;
		}
	      if((unsigned int)pointer+pointer->size==(unsigned int)temp)
		{
		  pointer->size+=temp->size;
		  pointer->next=temp->next;
		}
	      return 1;
	    }
	}
      pointer=m_next;
      m_next=m_next->next;
    }
  return 0;
}
//**************************************//

#include <memory_manage/memory_manage.h>
#include <task.h>
#include <memory.h>


//***************use in version 2015**************************//
void remap_page(unsigned int *pde_add,unsigned int line,unsigned int physicle)
{
  //unsigned int *pde_add=(unsigned int*)t_list->task_struct_add[pid]->tasc.cr3;
  unsigned int *pte_add=NULL;
  unsigned int index_of_pde=(line&0xffc00000)>>22;
  unsigned int index_of_pte=(line&0x3ff000)>>12;
  pte_add=(unsigned int*)(pde_add[index_of_pde]&0xfffff000);
  pde_add[index_of_pde]=pde_add[index_of_pde]|PAGE_WRITE|PAGE_USER|PAGE_PRESENT;
  pte_add[index_of_pte]=physicle|PAGE_PRESENT|PAGE_WRITE|PAGE_USER;
}
//**********************************************************//
//****************use in version 2015************************//
void free_remap(int pid,struct remap_list *l,unsigned int line)
{
  int i=0;
  while(l!=NULL)
    {
      for(;i<REMAP_LIST_LENGTH;i++)
	{
	  if( l->remap[i].pid==pid)
	    {
	      if(l->remap[i].line_add==line)
	      free_page(pid,line);
	      return;
	    }
	}
      l=l->next_list;
    }
}
//***********************************************************//
//*************use in version****************//
void free_remap_all(int pid,struct remap_list *l)
{
  int i=0;
  while(l!=NULL)
    {
      for(;i<REMAP_LIST_LENGTH;i++)
	{
	  if(l->remap[i].pid==pid)
	    {
	      free_page(pid,l->remap[i].line_add);
	    }
	}
    }
}
//*************************************************//
//******use in version 2015*************//
void init_remap_list(struct remap_list *list)
{
  int i=0;
  list->next_list=NULL;
  for(;i<REMAP_LIST_LENGTH;i++)
    {
      list->remap[i].pid=-1;
      list->remap[i].line_add=0;
    }
}
//**********************************//
//*******use in version 2015********//
int insert_remap_list(int pid,struct remap_list *l,unsigned int line)
{
  int i=0;
  //  int insert_flag=0;
  struct remap_list *temp_l=l;
  while(temp_l!=NULL)
    {
      for(;i<REMAP_LIST_LENGTH;i++)
	{
	  if(l->remap[i].pid==-1)
	    {
	      l->remap[i].pid=pid;
	      l->remap[i].line_add=line;
	      return 1;
	    }
	}
      temp_l=temp_l->next_list;
    }
  // remap list has no any space to put the remap info
  temp_l=l;
  //  alloc a new space to save the remap_list
  struct remap_list *another_list=(struct remap_list*)alloc_memory(KERNEL_SPACE,SIZE_OF_PAGE);

  // check the new allocted memory space
  if(another_list==NULL)
    {
      // another should not be NULL 
      return 0;
    }

  // initial the new allocted remap list 
  for(i=0;i<REMAP_LIST_LENGTH;i++)
    {
      another_list->remap[i].pid=-1;
    }

  //  search the lastest node of the remap list
  while(temp_l->next_list!=NULL)
    {
      temp_l=temp_l->next_list;
    }

  // add the new space into the remap_list
  temp_l->next_list=another_list;
  // put the remap info into the remap list
  another_list->remap[0].pid=pid;
  another_list->remap[0].line_add=line;
  return 1;
}
//**********************************//

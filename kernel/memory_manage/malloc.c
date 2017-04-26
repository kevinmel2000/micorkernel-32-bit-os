#include <memory.h>
#include <memory_manage/memory_manage.h>
#include <task.h>
/*************use int version 2015*************/
/*void *sys_malloc(int pid,unsigned int size)
{
  struct task *pro=t_list->task_struct_add[pid];
  unsigned int phy_add=0;
  unsigned int line_add=0;
  struct malloc_struct *m=NULL;
  if(pro->proce_memory.not_allocted==NULL)
    {
      phy_add=alloc_memory(USER_SPACE,SIZE_OF_PAGE);
      line_add=alloc_page(pid,phy_add);
      pro->proce_memory.not_allocted=(void*)line_add;
      m=(struct malloc_struct*)phy_add;
      m->size=SIZE_OF_PAGE;
      m->next=NULL;
    }
  else
    {
      line_add=(unsigned int)pro->proce_memory.not_allocted;
      phy_add=(unsigned int)line2physicle(pid,(unsigned int)pro->proce_memory.not_allocted);
    }
  unsigned int return_value=alloc(pid,phy_add,size+4);  /*  add 4 byte cause have to save the size of allocted 
  return_value=return_value&0xfff+line_add&0xfffff000;
  return (void*)return_value;
  }*/
/**********************************************/
/**********use in version 2015************/
/*unsigned int alloc(int pid,unsigned int phy_add,unsigned int size)
{
  struct task *pro=t_list->task_struct_add[pid];
  unsigned int line=((unsigned int)(pro->proce_memory.not_allocted))&0xfffff000;
  struct malloc_struct *m=(struct malloc_struct*)phy_add;
  struct malloc_struct *m_next=m->next;
  unsigned int needed=0;
  unsigned int temp_size=0;
  struct malloc_struct *temp_next;
  unsigned int *put_size=NULL;

  //  m is OK alloc m //
  if(m->size>=size)
    {
      needed=(unsigned int)m;
      temp_next=m->next;
      temp_size=m->size;
      m=(struct malloc_struct*)(needed+size);
      m->size=temp_size-size;
      m->next=temp_next;
      pro->proce_memory.not_allocted=(void*)(line+(((unsigned int)m)&0xfff));
      put_size=(unsigned int*)needed;
      *put_size=size;
      return needed+4;
    }
  //////////////////////


  // m is not OK but it has a next //
  if(m_next!=NULL)
    {
      while(m_next!=NULL)
	{
	  if(m_next->size>size)
	    {
	      needed=(unsigned int)m_next;
	      temp_next=m_next->next;
	      temp_size=m_next->size;
	      m_next=(struct malloc_struct*)(((unsigned int )m_next)+size);
	      m_next->size=temp_size-size;
	      m_next->next=temp_next;
	      m->next=m_next;
	      put_size=(unsigned int*)needed;
	      *put_size=size;
	      return needed+4;
	    }
	  else if(m_next->size==size) 
	    {
	      needed=(unsigned int)m_next;
	      unsigned int *put_size=(unsigned int*)needed;
	      *put_size=size;
	      m->next=m_next->next;
	      return needed+4;
	    }
	  m=m_next;
	  m_next=m_next->next;
	}
    }
  ///////////////////////////////////

  // m is not OK and had no next  //
  return 0;
  /////////////////////////////////
  }*/
/***********************************************/

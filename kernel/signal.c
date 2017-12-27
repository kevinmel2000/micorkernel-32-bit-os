#include <myos.h>
#include <signal.h>
#include <task.h>
#include <memory.h>
// default signal handler, do NOTHING
void dft_signal_handle()
{
  return;
}

// find the specifeic signal handler
void* find_signal_handler(int pid,unsigned int signal)
{
  int i;
  struct task *pro=t_list->task_struct_add[pid];
  unsigned int signal_value=pro->signal_mask&signal;
  struct signal_handle *handler_list=pro->signal_handler;
  for(i=0;i<32;i++)
    {
      // find which signal handler will called
      if(((signal_value>>i)&1)!=0)
	{
	  if(handler_list[i].redefine_handler==NULL)
	    {
	      // redefine the default handler
	      return handler_list[i].handler;
	    }
	  // return the redefine handler
	  return handler_list[i].redefine_handler;
	}
    }
  return NULL;
}

// set signal handler for each process
void init_signal_handler(struct signal_handle *l)
{
  int i;
  for(i=0;i<SIGHANDLELIST_SIZE;i++)
    {
      l[i].handler=dft_signal_handle;
      l[i].redefine_handler=NULL;
    }
}

// set signal handler for pm proce
void init_signal_wait_list(struct signal_wait_list *l)
{
  int i;
  for(i=0;i<SIGWAITLIST_SIZE;i++)
    {
      l[i].pid=-1;
      l[i].signal=0;
    }
}

// set which pro is waitting what signal
int set_signal_wait(int pid,unsigned int signal,struct signal_wait_list *l)
{
  int i=0;
  for(;i<SIGWAITLIST_SIZE;i++)
    {
      if(l[i].pid==-1)
	{
	  l[i].pid=pid;
	  l[i].signal=signal;
	  return i;
	}
    }
  return -1;
}

// get this process is waitting what signal
int waitting_this_signal(int pid,unsigned int signal,struct signal_wait_list *l)
{
  int i=0;
  for(;i<SIGWAITLIST_SIZE;i++)
    {
      if(l[i].pid==pid)
	{
	  if(l[i].signal==signal)
	    {
	      return i;
	    }
	}
    }
  return -1;
}

// clear what this peocess want from signal_wait_list
void clear_signal_wait_list(int pid,struct signal_wait_list *l)
{
  int i=0;
  for(;i<SIGWAITLIST_SIZE;i++)
    {
      if(l[i].pid==pid)
	{
	  l[i].pid=-1;
	  l[i].signal=0;
	}
    }
}

void do_signal(int pid,unsigned int signal)
{
  //fix the user stack
  //change the kernel level stack
  //change ip point to signal_handle_entry() function

  int i=0;
  // get specifiec process handler funtion
  struct task *pro=t_list->task_struct_add[pid];
  if(pro==NULL)
    {
      return;
    }
  void *handler=find_signal_handler(pid,signal);
  if(handler==NULL)
    {
      // should not be NULL
      while(1);
    }
  //the speciefic proce is waitting for a message,
  //  OK,then everything is easy
  if(pro->msg_m.status==WAIT_RECEIVE)
    {
      struct message msg;
      msg.type=SEND;
      msg.function=HAD_SIGNAL;
      msg.return_value=(unsigned int)handler;
      send_message(&msg);
    }

  // speciefic is running, we should reset the stack space
  // get ring0 level esp pointer
  unsigned int line_kernel_esp=pro->tasc.esp;

  // get physicle address of esp
  unsigned int *physicle_kernel_esp=(unsigned int*)line2physicle(pid,line_kernel_esp);

  // get esp position
  physicle_kernel_esp=(unsigned int*)((unsigned int)physicle_kernel_esp+0x64);

  //now:
  // stack_value[0]=edi,[1]=esi,[2]=ebp,[3]=esp,
  //[4]=ebx,[5]=edx,[6]=ecx,[7]=eax,[8]=ds,[9]=es,
  //[a]=nouse,[b]=ip,[c]=cs,[d]=iflag,[e]=esp,[f]=ss
  unsigned int stack_value[16];
  for(i=0;i<16;i++)
    {
      stack_value[i]=physicle_kernel_esp[i];
    }
  physicle_kernel_esp[7]=(unsigned int)handler;

  // get the user eip
  unsigned int ret_eip=physicle_kernel_esp[0xb];
  physicle_kernel_esp[0xb]=(unsigned int)signal_handler_entry;

  // reset user stack esp
  physicle_kernel_esp[0xe]-=36;
  unsigned int *physicle_user_esp=(unsigned int*)line2physicle(pid,physicle_kernel_esp[0xe]);
  for(i=0;i<8;i++)
    {
      // set the value of return to user
      physicle_user_esp[i]=stack_value[i];
    }
  physicle_user_esp[8]=ret_eip;
}

// set yourself sigaction
void redefine_handler(int pid,unsigned int signal,void *function)
{
  int i=0;
  struct task *pro=t_list->task_struct_add[pid];
  for(;i<SIGHANDLELIST_SIZE;i++)
    {
      if((signal>>i)&1)
	{
	  pro->signal_handler[i].redefine_handler=function;
	  return;
	}
    }
}

// reset sigaction
void recover_handler(int pid,unsigned int signal)
{
  redefine_handler(pid,signal,NULL);
}

#include <IPC/message.h>
#include <arch_x86/interrupt/interrupt.h>
#include <memory.h>
#include <task.h>
#include <kernel.h>
#define PM_TASK 1
// if the process sending the message call this 
void sys_send_message(struct message *msg)
{
  struct task *dst=t_list->task_struct_add[msg->dst];
  struct task *src=t_list->task_struct_add[msg->src];
  while(save_message(msg)==-1)
    {
      //  the mail box if full lock the send process
      lock(msg->src);
    }
  if(dst!=NULL&&(dst->msg_m.status==WAIT_RECEIVE&&(dst->msg_m.want_receive_from==msg->src||dst->msg_m.want_receive_from==ANY)))
    {
      dst->msg_m.want_receive_from=0;
      dst->msg_m.status=NEED_NOTHING;
      unlock(msg->dst);    //  unlock the process want to receive message
      return;
    }
}
// if the process want to receive message call this
void sys_receive_message(struct message *msg)
{
  struct task *dst=t_list->task_struct_add[msg->dst];
  struct task *src=t_list->task_struct_add[msg->src];
  struct message *needed;
  while(IS_EMPTY(src)||(needed=get_message(msg))==NULL)
    {
      //  can't receive any message from other process
      dst->msg_m.status=WAIT_RECEIVE;
      dst->msg_m.want_receive_from=msg->src;
      dst->msg_m.msg=msg;
      lock(msg->dst);
    }
  //  received message from other process
  copy_message(needed,msg);
  dst->msg_m.want_receive_from=0;
  dst->msg_m.status=NEED_NOTHING;
  dst->msg_m.msg=NULL;
}
void copy_message(struct message *src,struct message *dst)
{
  copy_memory((char*)src,(char*)dst,sizeof(struct message));
}
//  called by software interrupt function
void send_recv(struct message *msg)
{
  unsigned int pid=t_list->now_runing;
  unsigned int msg_add=(unsigned int)msg;
  msg_add=virtul2line(pid,msg_add);
  struct message *message=(struct message*)msg_add;
  if(message->type==SEND)
    {
      message->src=pid;
      sys_send_message(message);
    }
  else if(message->type==RECEIVE)
    {
      message->dst=pid;
      sys_receive_message(message);
    }
  else if(message->type==ROUND_TRIP)
    {
      message->type=SEND;
      message->src=pid;
      sys_send_message(message);
      message->type=RECEIVE;
      message->src=message->dst;
      message->dst=pid;
      sys_receive_message(message);
    }
}
//  put the message into the mailbox
int save_message(struct message *msg)
{
  int i=0;
  struct message_buffer *temp=message_empty;
  if(IS_EMPTY(temp))
    {
      return -1;
    }
  message_empty=temp->next;
  copy_message(msg,&temp->msg);
  temp->next=message_send;
  message_send=temp;
  return 0;
}

//  get message from mailbox
struct message *get_message(struct message *msg)
{
  msg->dst=t_list->now_runing;
  int i=0;
  struct message *needed=NULL;
  struct message_buffer *temp=message_send;
  struct message_buffer *empty=NULL;
  if(temp!=NULL)
    {
      // THE MESSAGE DST IS THIS TASK
      if(temp->msg.type==SEND&&temp->msg.dst==msg->dst)
	{
	  // THIS TASK WANT MSG FROM ANY TASK OR WANT THE SPECIFIEC TASK OR IT IS SEND FROM PM_TASK,THE PM_TASK WILL TELL THIS PROCESS TO RUN A SIGNAL HANDLER
	  if(msg->src==ANY||msg->src==temp->msg.src||temp->msg.src==PM_TASK)
	    {
	      needed=&temp->msg;
	      message_send=temp->next;
	      temp->next=message_empty;
	      message_empty=temp;
	      return needed;
	    }
	}
    }
  while(temp!=NULL&&temp->next!=NULL)
    {
      if(temp->next->msg.dst==msg->dst)
	{
	  if(temp->next->msg.src==msg->src||msg->src==ANY)
	    {
	      needed=&temp->next->msg;
	      empty=temp->next;
	      temp->next=empty->next;
	      empty->next=message_empty;
	      message_empty=empty;
	      return needed;
	    }
	}
      temp=temp->next;
    }
  return NULL;
}
//  init the mailbox and the send queue,empty queue
void init_message()
{
  int i=0;
  message_list=(struct message_buffer*)alloc_memory(KERNEL_SPACE,SIZE_OF_PAGE);
  message_empty=message_list;
  message_send=NULL;
  for(;i<MSG_BUF_SIZE;i++)
    {
      message_empty[i].next=&message_empty[i+1];
    }
  message_empty[i-1].next=NULL;
  set_handler(0x40,send_recv);
}
//  clear the message about the pid from the send queue
//  what ever the pid send or the dst is pid
void clear_message(int pid)
{
  struct message_buffer *buffer=message_send;
  if(IS_EMPTY(buffer))
    {
      return;
    }
  while(buffer->next!=NULL)
    {
      if(buffer->next->msg.dst==pid||buffer->next->msg.src==pid)
	{
	  struct message_buffer *temp=buffer->next;
	  buffer->next=buffer->next->next;
	  temp->next=message_empty;
	  message_empty=temp;
	}
      buffer=buffer->next;
    }
  buffer=message_send;
  if(buffer->msg.src==pid||buffer->msg.dst==pid)
    {
      message_send=buffer->next;
      buffer->next=message_empty;
      message_empty=buffer;
    }
}

void receive_message(struct message *msg)
{
  msg->type=RECEIVE;
  int40(msg);
  if(msg->function==HAD_SIGNAL)
    {
      void (*signal_handler)()=(void(*))msg->return_value;
      signal_handler();
    }
}

void round_message(struct message *msg)
{
  msg->type=ROUND_TRIP;
  int40(msg);
  if(msg->function==HAD_SIGNAL)
    {
      void (*signal_handler)()=(void(*))msg->return_value;
      signal_handler();
    }
}

void send_message(struct message *msg)
{
  msg->type=SEND;
  int40(msg);
}

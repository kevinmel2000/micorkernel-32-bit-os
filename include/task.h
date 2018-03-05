#ifndef TASK_H
#define TASK_H
#include <arch_x86/arch_x86.h>
#include <IPC/message.h>
#include <memory.h>
#include <signal.h>

#define LDT_DESCRIB   0x82   
#define TSS_DESCRIB   0x89  
#define LDT_SELECTOR  0x4    
#define SERVICE_AR    1      
#define TSS_SIZE      103     
#define LDT_SIZE      4      
#define VIDEO_GDT     3
#define VIDEO_LDT     2
#define DS_LDT        0      
#define DS_GDT        1     
#define CS_GDT        2    
#define CS_LDT        1     

//    TASK INITIALIZE // 
#define NORMAL_SIZE   0x1000
#define NORMAL_IOMAPBASE 0x490000
#define OK            1
#define NOT           0
#define NULL          0
#define USER_AR       0x3
#define OS            0x940321
#define USER          0xffffff
#define MAX_TASK_NUMBER 256
#define RUNNING 1
#define HANG_UP 2
#define BLOCK   4
struct tss
{
  unsigned int pretask,esp0,ss0,esp1,ss1,esp2,ss2;
  unsigned int cr3,eip,eflags,eax,ecx,edx,ebx,esp,ebp,esi,edi;
  unsigned int es,cs,ss,ds,fs,gs,ldt,iomap_base;
};
struct task
{
  ldt this_ldt[LDT_SIZE];
  struct tss tasc;
  unsigned int selector,user_esp,next,status,time,entry,signal_mask;
  int father,child,brother;
  struct proce_memory_struct proce_memory;
  struct message_manager msg_m;
  struct signal_handle signal_handler[SIGHANDLELIST_SIZE];
};
struct task_manager
{
  struct task *task_struct_add[MAX_TASK_NUMBER];
  unsigned int task_number; 
  int now_runing;
  int counter;
};

extern void reset_tasks();
extern void set_kernel_task();
extern void lock(unsigned int pid);
extern void unlock(unsigned int pid);
extern int set_task(void (*task)(),unsigned priority);
extern unsigned int find_empty_process();
extern unsigned int find_empty_gdt();
extern void free_process(unsigned int pid);
extern void free_gdt(unsigned int gdt);
extern void set_driver_list();
struct task_manager *t_list;
/*#define no_page_nf() asm("movl %cr0,%eax\n"\			
                      "andl $0x7fffffff,%eax\n"\		
		      "movl %eax,%cr0")
#define set_page_nf() asm("movl %cr0,%eax\n"\
		       "or   $0x80000001,%eax\n"\
		       "movl %eax,%cr0")
#define no_page_trans_add_nf() asm("shll $2,%%ebx\n"\   
                        "addl %%ebx,%%eax\n"\  
			"movl (%%eax),%%eax\n"\  
			"movl %%eax,%%esi\n"  \
			"movl %%eax,%%ebx\n"\
			"movl %%esp,140(%%ebx)\n"\ 
			"movl 60(%%esi),%%esi\n"\     
			"movl %%esp,%%eax\n"	\
			"movl %%eax,%%edx\n"	\
			"movl %%ebp,%%edi\n"	\
			"subl %%eax,%%edi\n"	\
			"andl $0xfffff000,%%eax\n"	\
			"movl %%eax,%%ecx\n"		\
			"movl %%eax,%%ebx\n"		\
			"andl $0xffa00000,%%ebx\n"	\
			"shrl $20,%%ebx\n"		\
			"addl %%ebx,%%esi\n"		\
			"movl (%%esi),%%esi\n"		\
			"andl $0xfffff000,%%esi\n"	\
			"andl $0x3ff000,%%ecx\n"	\
			"shrl $10,%%ecx\n"		\
			"addl %%ecx,%%esi\n"		\
			"movl (%%esi),%%esi\n"		\
			"andl $0xfffff000,%%esi\n"	\
			"andl $0xfff,%%edx\n"		\
			"addl %%edx,%%esi\n"		\
			"addl %%esi,%%edi\n"		\
			"movl %%esi,%%esp\n"		\
			"movl %%edi,%%ebp"		\
			:				\
			:"a"(t_list),"b"(t_list->now_runing));
#define set_page_trans_add_nf() asm("shl $2,%%ebx\n"\
                                 "addl %%ebx,%%eax\n"\
                                 "movl (%%eax),%%eax\n"\
                                 "movl 140(%%eax),%%eax\n"\
                                 "andl $0xfffff000,%%eax\n"\
                                 "movl %%esp,%%esi\n"\
                                 "movl %%ebp,%%edi\n"\
                                 "subl %%esi,%%edi\n"\
                                 "andl $0xfff,%%esi\n"\
                                 "or   %%eax,%%esi\n"\
                                 "addl %%esi,%%edi\n"\
                                 "movl %%esi,%%esp\n"\
                                 "movl %%edi,%%ebp"\
                                 :		    \
				 :"a"(t_list),"b"(t_list->now_runing));*/
int stack_top;
int before_proc;
int pre_task;
int is_init_fs;
#endif

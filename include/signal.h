#ifndef SIGNAL_H 
#define SIGNAL_H

// signal definition
#define SIGUP       0x1
#define SIGINT      0x2
#define SIGQUIT     0x4
#define SIGILL      0x8
#define SIGTRAP     0x10
#define SIGABRT     0x20
#define SIGIOT      0x40
#define SIGUNUSED   0x80
#define SIGFPE      0x100
#define SIGKILL     0x200
#define SIGEGV      0x300
#define SIGPIPE     0x400
#define SIGALRM     0x800
#define SIGTERM     0x1000
#define SIGSTKFLT   0x2000
#define SIGCHLD     0x4000
#define SIGCOUNT    0x8000
#define SIGSTOP     0x10000

#define SIGHANDLELIST_SIZE 18

#define SIGWAITLIST_SIZE 0x200


//process signal handle list 
struct signal_handle
{
  void (*handler)();
  void (*redefine_handler)();
};

// process signal waiting list
struct signal_wait_list
{
  int pid;
  unsigned int signal;
};

extern void dft_signal_handle();
extern void* find_signal_handler(int pid,unsigned int signal);
extern void signal_handler_entry();
extern void init_signal_handler(struct signal_handle *l);
extern void init_signal_wait_list(struct signal_wait_list *l);
extern int set_signal_wait(int pid,unsigned int signal,struct signal_wait_list *l);
extern int waitting_this_signal(int pid,unsigned int signal,struct signal_wait_list *l);
extern void clear_signal_wait_list(int pid,struct signal_wait_list *l);
extern void do_signal(int pid,unsigned int signal);
/**asm function**/
extern void signal_handler_entry();
/****************/
extern void redefine_handler(int pid,unsigned int signal,void *function);
extern void recover_handler(int pid,unsigned int signal);
#endif

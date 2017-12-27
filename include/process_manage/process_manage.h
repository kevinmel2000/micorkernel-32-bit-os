#ifndef PM_H
#define PM_H
#define FORK 0x6000
#define EXIT ~FORK
#define EXEC 0x7000
#define SIGNAL 0x8000
#define NO_SINGAL ~SIGNAL
#define REDEF_SIGNAL 0x8001
#define RECOV_SIGNAL ~REDEF_SIGNAL

#include <../signal.h>

extern void process_manage();
extern void set_pm_task();
extern int do_fork(unsigned int father);
extern void do_exit(int pid,struct signal_wait_list *l);
extern void free_process_memory(int pid);
#endif

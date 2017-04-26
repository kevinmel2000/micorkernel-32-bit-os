#ifndef MM_H
#define MM_H 
#define REMAP 0x800000
#define NO_REMAP ~REMAP 
#define MALLOC 0x800001
#define FREE ~MALLOC
#include <memory.h>
extern void memory_manage();
extern void set_mm_task();
//extern void *sys_malloc(int pid,unsigned int size);
//extern unsigned int alloc(int pid,unsigned int phy_add,unsigned int size);
//extern void remap_page(unsigned int *pde_add,unsigned int line,unsigned int physicle);
extern void free_remap(int pid,struct remap_list *l,unsigned int line);
extern void free_remap_all(int pid,struct remap_list *l);
extern void init_remap_list(struct remap_list *list);
extern int insert_remap_list(int pid,struct remap_list *l,unsigned int line);
#endif

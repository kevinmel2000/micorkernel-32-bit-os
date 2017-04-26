#ifndef MEMORY_H
#define MEMORY_H
#define ROOM_SIZE 1024
#define PAGE_PRESENT 1
#define PAGE_READ 0
#define PAGE_WRITE 2
#define PAGE_USER 4
#define PAGE_SUPER 0
#define SIZE_OF_PAGE 0x1000
#define KERNEL_SPACE 0x1234
#define USER_SPACE   0x5678
#define REMAP_LIST_LENGTH 511
#define REFRESH_TLB() asm("movl %cr3,%eax\n\
                           movl %eax,%cr3")
struct memory_room
{
  unsigned int base;
  unsigned int size;
};
struct memory_manage
{
  struct memory_room room[ROOM_SIZE];
  unsigned int base;
};
struct proce_memory_struct
{
  void *not_allocted;
  void *sbrk;
};
struct malloc_struct
{
  unsigned int size;
  unsigned int line_add;
  struct malloc_struct *next;
};
struct remap_struct
{
  int pid;
  unsigned int line_add;
};
struct remap_list
{
  struct remap_struct remap[REMAP_LIST_LENGTH];
  struct remap_list *next_list;
};
extern void init_memory(unsigned int memory_size,unsigned int free_memory_add,unsigned int free_memory_add_high);
extern unsigned int check_memory();
extern void set_page(unsigned int memory_size);
//extern void set_memory(int addr,int msg);
//extern int get_memory(int addr);
extern void copy_memory(char *src,char *aim,unsigned int size);
//extern void write_memory(void *buf,char data,int size);
//extern unsigned int alloc_stack(int size);
extern unsigned int alloc_memory(unsigned int type,unsigned int size);
extern void free_memory(unsigned int base,unsigned int size);
//extern void free_stack(unsigned int base,int size);
extern unsigned int alloc_page(int pid,unsigned int physicle_add);
extern void free_page(int pid,unsigned int line_add);
//extern int handle_no_page(unsigned int line_add);
//extern unsigned int get_error_add();
//extern unsigned int my_malloc(unsigned int size);
//extern void my_free(unsigned int* add);
//extern void store_page(unsigned int page_add);
//extern void set_pdte(unsigned int add,unsigned int data,unsigned int size);
extern unsigned int virtu2line(unsigned int pid,unsigned int virtul_add);
extern unsigned int line2physicle(unsigned int pid,unsigned int line_add);
//extern void copy_stack(unsigned int *src,unsigned int *dst,unsigned int size);
extern void page_handler(unsigned int error_code,unsigned int error_eip);
extern void set_page_write(unsigned int error_add);
extern void remap_page(unsigned int *pde_add,unsigned int line,unsigned int physicle);
extern void *sys_malloc(int pid,unsigned int size);
extern unsigned int alloc(int pid,struct malloc_struct *m,unsigned int size);
extern int sys_free(int pid,unsigned int *line_add);
//extern void set_page_use(unsigned int *pde_add,unsigned int line_add);
struct memory_manage *memory_list;
struct remap_list *remap_l;
unsigned int pde_add;
unsigned int memory_size;
unsigned int free_memory_add;
unsigned int free_memory_add_high;  
unsigned int kernel_page_add;
#endif

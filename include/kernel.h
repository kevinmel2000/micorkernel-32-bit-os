#ifndef KERNEL_H
#define KERNEL_H
#define NULL        0
#define RUNNING     1 
#define BLOCKED     2
//#define WAIT_MSG    4
//#define WAIT_SEND   5
//#define SEND        6
//#define RECEIVE     7
//#define ROUND_TRIP  8
//#define INTERRUPT   9
//#define KNOW_MY_HD  10
//#define EMPTY       0xffffffff
/////////////////////////////////////////
#define KERNEL_ADD      0x500
#define KERNEL_SIZE_ADD 0x502 
#define DRIVER_NUM      0x504
#define DRIVER_DESC_ADD 0x600
#define FS              0x1
#define HD              0x2
#define TTY             0x4
#define CONSOLE         0x8
#define KEYBOARD        0x10
#define SHELL           0x20
#define STRUCTS_ADD     0x100000
#define MAX_DRIVER_NUM      64
struct driver_descrip
{
  unsigned int type;
  void *add;
};
struct driver_load_manage
{
  struct driver_descrip drivers[MAX_DRIVER_NUM];
  unsigned int driver_num;
};
// save the size of kernel in memory
unsigned int kernel_size;
struct driver_load_manage *driver_list;
#endif

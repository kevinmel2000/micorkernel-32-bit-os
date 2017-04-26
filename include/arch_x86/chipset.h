#ifndef CHIPSET_H
#define CHIPSET_H
extern void init_pic();
extern void set_clock();
extern void clock_handler(int irq_num);
#endif

#include <kernel.h>
#include <task.h>
#include <arch_x86/chipset.h>
#include <arch_x86/interrupt/interrupt.h>
void init_pic()
{
  out(0x21,  0xff  );
  out(0xa1,  0xff  );
  out(0x20, 0x11  );
  out(0x21, 0x20  );
  out(0x21, 1 << 2);
  out(0x21, 0x01  );
  out(0xa0, 0x11  );
  out(0xa1, 0x28  );
  out(0xa1, 2     );
  out(0xa1, 0x01  );
  out(0x21,0);
  out(0xa1,0);
}
void set_clock()
{
  out(0x43,0x34);
  out(0x40,0x9c);
  out(0x40,0x2e);
  set_handler(0x20,clock_handler);
}
void clock_handler(int irq_num)
{
  t_list->counter--;
  if(t_list->counter==0)
    {
      switchtask();
    }
}

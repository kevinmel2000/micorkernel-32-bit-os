#include <elf.h>
struct program_header *get_program_header(void *program)
{
  struct elf_header *eh=(struct elf_header*)program;
  unsigned int temp=(unsigned int)program;
  temp+=eh->phoff;
  return (struct program_header*)temp;
}
void *get_program_add(void *program,unsigned int *size)
{
  struct program_headder *ph=get_program_header(program);
  *size=ph->file_size;
  unsigned int temp=(unsigned int)program+ph->offset; 
  return (void*)temp;
}
void *get_entry_point(void *program)
{
  struct prgram_header *ph=get_program_header(program);
  return ph->virtu_add;
}

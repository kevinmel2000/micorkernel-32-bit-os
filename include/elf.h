#ifndef ELF_H
#define ELF_H
#define NAME_LENGTH 16
struct elf_header
{
  char ident_name[NAME_LENGTH];
  unsigned short type;
  unsigned short machine;
  unsigned int version; 
  unsigned int entry;
  unsigned int phoff;
  unsigned int shoff;
  unsigned int flags;
  unsigned int ehsize;
  unsigned int phentsize;
  unsigned int phnum;
  unsigned int shentsize;
  unsigned int shnum;
  unsigned int shstrndx;
};
struct program_header
{
  unsigned int type;
  unsigned int offset;
  unsigned int virtu_add;
  unsigned int physical_add;
  unsigned int file_size;
  unsigned int memory_size;
  unsigned int flag;
  unsigned int align;
};
extern struct program_header*get_program_header(void *program);
extern void *get_program_add(void *program,unsigned int *size);
extern void *get_entry_point(void *program);
#endif

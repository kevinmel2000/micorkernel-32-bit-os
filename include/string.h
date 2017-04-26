#ifndef STRING_H
#define STRING_H
#define HEX 16
#define DEC 10
#define BIN 2
extern int strcmp(char *str1,char *str2);
extern int strcmp_length(char *str1,char *str2,int start,int length);
extern int strcpy(char *src,char *dst); 
extern int atoi_by_length(int system,char *str,int length);
extern int atoi(int system,char *str);
extern void itoa(int system,int number,char *str);
extern unsigned int strlen(char *str);
extern void strcat(char *str1,char *str2);
#endif

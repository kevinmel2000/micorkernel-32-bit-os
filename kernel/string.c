#include <string.h>
int atoi_by_length(int system,char *str,int length)
{
  int number=0,i=0;
  if(length<=0||length>8)
    {
      return -1;
    }
  switch(system)
    {
    case HEX:
      for(;i<length;i++)
	{
	  number=(number<<4)|(str[i]&0xf);
	}
      break;
    case DEC:
      for(;i<length;i++)
	{
	  number=number*10+(str[i]&0xf);
	}
      break;
    case BIN:
      for(;i<length;i++)
	{
	  number=(number<<1)|(str[i]&1);
	}
      break;
    }
  return number;
}
int atoi(int system,char *str)
{
  int number=0,i=0;
  switch(system)
    {
    case HEX:
      for(;str[i]!='\0';i++)
	{
	  number=(number<<4)+(str[i]&0xf);
	}
      break;
    case DEC:
      for(;str[i]!='\0';i++)
	{
	  number=number*10+(str[i]&0xf);
	}
      break;
    case BIN:
      for(;str[i]!='\0';i++)
	{
	  number=(number<<1)|(str[i]&1);
	}
      break;
    }
  return number;
}
void itoa(int system,int number,char *str)
{
  int i=0;
  int temp=0;
  if(system==HEX)
    {
      for(;i<8;i++)
	{
	  str[i]=0x30|((number>>((7-i)*4))&0xf);
	}
    }
  else if(system==DEC)
    {
      for(i=1000000000;i>=10;i=i/10)
	{
	  str[temp]=0x30|(number/i);
	  number%=i;
	  temp++;
	}
	str[temp++]=0x30|(number%10);
	str[temp]='\0';
    }
}
unsigned int strlen(char *str)
{
  int i=0;
  for(;str[i]!='\0';i++);
  return i;
}
void strcat(char *str1,char *str2)
{
  int i,j;
  for(i=0;str1[i]!='\0';i++);
  for(j=0;str2[j]!='\0';j++)
    {
      str1[i]=str2[j];
      i++;
    }
}

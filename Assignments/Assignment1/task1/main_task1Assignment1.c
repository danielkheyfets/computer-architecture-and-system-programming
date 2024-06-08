#include <stdio.h>

extern void assFunc(int x);

int main(int argc, char** argv)
{
  char buf[sizeof(int)];
  fgets(buf, sizeof(int), stdin);
  int num;
  sscanf(buf,"%d",&num);
  assFunc(num);
  return 0;
}

char c_checkValidity(int x)
{
    return (x%2) == 0 ? '1' : '0';
}
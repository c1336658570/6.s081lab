#include "kernel/param.h"
#include "kernel/stat.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char *arg[MAXARG];
  char buf[1024];

  int i = 1, count = 0;
  while (argv[i]) {
    arg[count++] = argv[i++];
  }

  char *ptr = buf;  //从键盘中读取
  char *a = ptr;    //指向读取的那个字符串的头部
  while (read(0, ptr, 1) == 1) {
    if (*ptr == '\n') {
      *ptr = '\0';
      arg[count++] = a;
      arg[count] = 0;
      a = ptr + 1;
    } else if (*ptr == ' ') {
      *ptr = '\0';
      arg[count++] = a;
      a = ptr + 1;
    }

    ptr++;
  }

  int pid = fork();
  if (pid == 0) {
    exec(arg[0], arg);
  } else {
    while (wait(0) != -1)
      ;
  }
  exit(0);
}

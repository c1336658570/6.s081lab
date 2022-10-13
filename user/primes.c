#include "../kernel/stat.h"
#include "../kernel/types.h"
#include "../user/user.h"

int main() {
  int pfd1[15][2], count = 0;
  pipe(pfd1[count++]);
  int pid = fork();
  if (pid == 0) {
    int i, num;
    close(pfd1[0][1]);
    while (1) {
      int ret = read(pfd1[0][0], &num, sizeof(num));
      if (ret == 0) {
        close(pfd1[0][0]);  //关闭读端
        break;
      }

      for (i = 2; i <= num / 2; ++i) {
        if (num % i == 0) {
          break;
        }
      }
      if (i == num / 2 + 1) {
        pipe(pfd1[count++]);
        pid = fork();
        if (pid == 0) {
          close(pfd1[0][0]);          //关闭读端
          close(pfd1[count - 1][1]);  //关闭写端
          read(pfd1[count - 1][0], &num, sizeof(num));
          close(pfd1[count - 1][0]);  //关闭读端
          printf("prime %d\n", num);
          exit(0);
        } else {
          close(pfd1[count - 1][0]);  //关闭读端
          write(pfd1[count - 1][1], &num, sizeof(num));
          close(pfd1[count - 1][1]);  //关闭写端
          wait(0);
        }
      }
    }
    exit(0);
  } else {
    int i;
    close(pfd1[0][0]);
    for (i = 2; i <= 35; i++) {
      write(pfd1[0][1], &i, sizeof(i));
    }

    close(pfd1[0][1]);
    wait(0);
    exit(0);
  }
}
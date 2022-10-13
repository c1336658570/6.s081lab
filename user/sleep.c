#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("please pass a parameter\n");  //没传递参数的话直接退出程序
    exit(0);
  }

  sleep(atoi(argv[1]));

  exit(0);
}

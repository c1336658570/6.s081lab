#include "user/user.h"

int main(int argc, char *argv) {
  int pfd1[2], pfd2[2];
  pipe(pfd1);
  pipe(pfd2);

  int pid = fork();
  if (pid == 0) {
    char ch = '1';
    close(pfd1[1]);
    close(pfd2[0]);
    read(pfd1[0], &ch, 1);
    printf("%d: received ping\n", getpid());
    write(pfd2[1], &ch, 1);
    close(pfd1[0]);
    close(pfd2[1]);
  } else {
    char ch = '1';
    close(pfd1[0]);
    close(pfd2[1]);
    write(pfd1[1], &ch, 1);
    read(pfd2[0], &ch, 1);
    printf("%d: received pong\n", getpid());
    close(pfd1[1]);
    close(pfd2[0]);
  }

  exit(0);
}
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
  pid_t pid1, pid2;

  pid1 = fork();
  if (pid1 < 0) {
    fprintf(stderr, "Fork Failed!\n");
    return 1;
  }
  else if (pid1 == 0) {
    printf("b\n");
    return 0;
  }
  else {
    pid2 = fork();
    if (pid2 < 0) {
      fprintf(stderr, "Fork Failed!\n");
      return 1;
    }
    else if (pid2 == 0) {
      printf("a\n");
      return 0;
    }
    else {
      printf("c\n");
      return 0;
    }
  }

  return 0;
}

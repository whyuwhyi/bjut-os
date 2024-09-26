#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char **argv) {
  pid_t pid1, pid2;
  char buf[200];

  int fds[2];
  if (pipe(fds) == -1) {
    fprintf(stderr, "Create Pipe Failed!\n");
    return -1;
  }


  pid1 = fork();
  if (pid1 == -1) {
    fprintf(stderr, "Fork Failed!\n");
    return -1;
  }
  else if (pid1 == 0) {
    sprintf(buf, "[%d]:Child1 is sending a message.\n", getpid());
    close(fds[0]);
    write(fds[1], buf, strlen(buf));
    return 0;
  }
  else {
    pid2 = fork();
    if (pid2 == -1) {
      fprintf(stderr, "Fork Failedi!\n");
      return -1;
    }
    else if (pid2 == 0) {
      sprintf(buf, "[%d]:Child2 is sending a message.\n", getpid());
      close(fds[0]);
      write(fds[1], buf, strlen(buf));
      return 0;
    }
    else {
      close(fds[1]);
      read(fds[0], buf, sizeof(buf));
      printf("%s", buf);

      close(fds[1]);
      read(fds[0], buf, sizeof(buf));
      printf("%s", buf);
      return 0;
    }
  }

  return 0;
}

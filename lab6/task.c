#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


static void signal_handler(int signo);

int main() {

    int pid = fork();

    if (pid == -1) {
        fprintf(stderr, "Fork failed!\n");
        return -1;
    }

    if (signal(SIGUSR1, signal_handler) == SIG_ERR) {
        printf("can't catch SIGUSR1\n");
        exit(1);
    }

    if (signal(SIGUSR2, signal_handler) == SIG_ERR) {
        printf("can't catch SIGUSR2\n");
        exit(1);
    }

    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        printf("can't catch SIGINT\n");
        exit(1);
    }

    for ( ; ; ) {
        pause();
    }

    return 0;
}


static void signal_handler(int signo) {
    printf("process[%d] received %d\n", getpid(), signo);
}
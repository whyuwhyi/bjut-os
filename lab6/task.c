#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

static void signal_handler (int signo);

int
main (int argc, char **argv)
{
  int pid = fork ();

  if (pid == -1)
  {
    fprintf (stderr, "Fork failed!\n");
    exit (1);
  }

  if (signal (SIGUSR1, signal_handler) == SIG_ERR)
  {
    printf ("can't catch SIGUSR1!\n");
    exit (1);
  }

  if (signal (SIGUSR2, signal_handler) == SIG_ERR)
  {
    printf ("can't catch SIGUSR2!\n");
    exit (1);
  }

  if (signal (SIGINT, signal_handler) == SIG_ERR)
  {
    printf ("can't catch SIGINT!\n");
    exit (1);
  }

  for ( ; ; ) 
    pause ();

  return 0;
}


static void
signal_handler (int signo)
{
  if (signo == SIGINT)
  {
    printf ("process[%d] received SIGINT.\nExit now!\n", getpid ());
    exit (0);
  }
  else
    printf ("process[%d] received %d.\n", getpid (), signo);
}
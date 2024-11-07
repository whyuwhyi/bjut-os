#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void thread1_function (void *arg);
void thread2_function (void *arg);

int
main (int argc, char **argv)
{
  int ret1, ret2;
  pthread_t myThread1, myThread2;

  ret1 = pthread_create (&myThread1, NULL, (void*) thread1_function, (void*)argv[1]);
  ret2 = pthread_create (&myThread2, NULL, (void*) thread2_function, (void*)argv[2]);

  if (ret1 || ret2)
  {
    fprintf (stderr, "Create Thread1 Failed!\n");
    exit (1);
  }

  pthread_join (myThread1, NULL);
  pthread_join (myThread2, NULL);

  return 0;
}

void
thread1_function (void *arg)
{
  char *ch;
  ch = (char*)arg;
  printf ("The charcter is %c.\n", *ch);
}

void
thread2_function (void *arg)
{
  int num;
  sscanf (arg, "%d", &num);
  printf ("The number is %d.\n", num);
}

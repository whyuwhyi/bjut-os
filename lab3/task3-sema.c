#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

int data[1000];

sem_t data_sem;

void init_array ();

void thread1_function ();
void thread2_function ();

int
main (int argc, char **argv)
{
  int ret1, ret2;
  pthread_t myThread1, myThread2;

  init_array ();
  sem_init (&data_sem, 0, 1);

  ret1 = pthread_create (&myThread1, NULL, (void*) thread1_function, NULL);
  ret2 = pthread_create (&myThread2, NULL, (void*) thread2_function, NULL);
  
  if (ret1 || ret2)
  {
    fprintf (stderr, "Create Thread1 Failed!\n");
    exit (1);
  }

  pthread_join (myThread1, NULL);
  pthread_join (myThread2, NULL);

  sem_destroy (&data_sem);
  
  return 0;
}

void
init_array ()
{
  for (int i = 0; i < 100; ++i)
    for (int j = 0; j < 10; ++j)
      data[i*10+j] = j+1;
}

void
thread1_function ()
{
  sem_wait (&data_sem);
  int sum = 0;
  for (int i = 0; i < 1000; ++i)
    sum += data[i];

  printf ("The sum of the one thousand data is %d.\n", sum);
  sem_post (&data_sem);
}

void
thread2_function ()
{
  sem_wait (&data_sem);
  for (int i = 0; i < 1000; ++i)
    for (int j = 998; j > i; --j)
    {
      if (data[j] > data[j+1])
      {
        int temp = data[j];
        data[j] = data[j+1];
        data[j+1] = temp;
      }
    }

  for (int i = 0; i < 1000; ++i)
    printf ("%d ", data[i]);
  printf ("\n");

  sem_post (&data_sem);
}
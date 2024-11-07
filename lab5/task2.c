#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>



#define NUM_INTS 10

struct shared_memory
  {
    int number;
    int flag;
  };

int
main (int argc, char **argv)
{
  int pid1 = fork ();

  if (pid1 == -1)
  {
    fprintf (stderr, "Fork failed!\n");
    exit (1);
  }
  else if (pid1 == 0)
  {
    key_t key = ftok ("task2.c", 65);
    int shmid = shmget (key, sizeof(struct shared_memory), 0666 | IPC_CREAT);

    if (shmid == -1)
    {
      fprintf (stderr, "Shared memory creation failed!\n");
      exit (1);
    }

    struct shared_memory *shm = (struct shared_memory*) shmat (shmid, NULL, 0);

    for (int i = 0; i < NUM_INTS; i++)
    {
      printf ("[%d]sender:Enter integer %d: ", getpid (), i+1);
      scanf ("%d", &shm->number);

      shm->flag = 1;

      while (shm->flag == 1);
    }

    shmdt (shm);
    shmctl (shmid, IPC_RMID, NULL);
  }
  else
  {
    int pid2 = fork ();

    if (pid2 == -1)
    {
      fprintf (stderr, "Fork failed!\n");
      exit (1);
    }
    else if (pid2 == 0)
    {
      sleep (1);

      key_t key = ftok ("task2.c", 65);
      int shmid = shmget (key, sizeof (struct shared_memory), 0666);

      if (shmid == -1)
      {
        fprintf (stderr, "Shared memory access failed!\n");
        exit (1);
      }

      struct shared_memory *shm = (struct shared_memory*)
                                  shmat (shmid, NULL, 0);

      for (int i = 0; i < NUM_INTS; i++)
      {
        while (shm->flag == 0);

        printf ("[%d]receiver:Number: %d, Square: %d\n", getpid (), 
                shm->number, shm->number * shm->number);

        shm->flag = 0;
      }

      shmdt (shm);
    }

    wait (NULL);
    wait (NULL);
  }

  return 0;
}
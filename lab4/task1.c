#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>

struct Buffer 
  {
    int data[2];
    int in;
    int out;
  };

void put (int data);
int get ();

void read1 ();
void read2 ();
void operate1 ();
void operate2 ();

struct Buffer buffer;
FILE *file1, *file2;
sem_t sem_buffer, sem_empty, sem_full;
int data, operator1, operator2, count;

int
main (int argc, char **argv)
{
  int ret1, ret2, ret3, ret4;
  pthread_t Reader1, Reader2, Operator1, Operator2;

  buffer.in = buffer.out = 0;
  count = 0;

  file1 = fopen ("1.dat", "r");
  file2 = fopen ("2.dat", "r");

  if (file1 == NULL || file2 == NULL)
  {
    fprintf (stderr, "File open failed!\n");
    exit (1);
  }
  
  sem_init (&sem_buffer, 0, 1);
  sem_init (&sem_empty, 0, 2);
  sem_init (&sem_full, 0, 0);

  ret1 = pthread_create (&Reader1, NULL, (void *) read1, NULL);
  ret2 = pthread_create (&Reader2, NULL, (void *) read2, NULL);
  ret3 = pthread_create (&Operator1, NULL, (void *) operate1, NULL);
  ret4 = pthread_create (&Operator2, NULL, (void *) operate2, NULL);

  if (ret1 || ret2 || ret3 || ret4)
  {
    fprintf(stderr, "Thread creation failed!\n");
    exit (1);
  }
  
  pthread_join (Reader1, NULL);
  pthread_join (Reader2, NULL);
  //pthread_join (Operator1, NULL);
  //pthread_join (Operator2, NULL);

  sem_destroy (&sem_buffer);
  sem_destroy (&sem_empty);
  sem_destroy (&sem_full);

  fclose (file1);
  fclose (file2);

  return 0;
}

void
read1 () 
{
  while (1) 
  {
    sem_wait (&sem_empty);
    sem_wait (&sem_buffer);
    if (fscanf (file1, "%d", &data) == EOF)
    {
      sem_post (&sem_buffer);
      sem_post (&sem_empty);
      return ;
    }
    put (data);
    if (++count == 2)
    {
      assert (count <= 2 && count > 0);
      sem_post (&sem_full);
    }
    sem_post (&sem_buffer);
  }
}

void 
read2 ()
{
  while (1)
  {
    sem_wait (&sem_empty);
    sem_wait (&sem_buffer);
    if (fscanf (file2, "%d", &data) == EOF)
    {
      sem_post (&sem_buffer);
      sem_post (&sem_empty);
      return ;
    }
    put (data);
    if (++count == 2)
    {
      assert (count <= 2 && count > 0);
      sem_post (&sem_full);
    }
    sem_post (&sem_buffer);
  }
}

void
operate1 ()
{
  while (1) {
    sem_wait (&sem_full);
    sem_wait (&sem_buffer);
    operator1 = get ();
    operator2 = get ();
    count = 0;
    printf ("%d + %d = %d\n", operator1, operator2, operator1 + operator2);
    sem_post (&sem_buffer);
    sem_post (&sem_empty);
    sem_post (&sem_empty);
  }
}

void
operate2 ()
{
  while (1) {
    sem_wait (&sem_full);
    sem_wait (&sem_buffer);
    operator1 = get ();
    operator2 = get ();
    count = 0;
    printf ("%d * %d = %d\n", operator1, operator2, operator1 * operator2);
    sem_post (&sem_buffer);
    sem_post (&sem_empty);
    sem_post (&sem_empty);
  }
}

void
put (int data)
{
  buffer.data[buffer.in] = data;
  buffer.in = (buffer.in + 1) % 2;
}

int
get ()
{
  int data = buffer.data[buffer.out];
  buffer.out = (buffer.out + 1) % 2;
  return data;
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int 
main (int argc, char **argv)
{
  pid_t pid1, pid2;

  pid1 = fork ();
  if (pid1 < 0)
  {
    fprintf (stderr, "Fork Failed!\n");
    exit (1);
  }
  else if (pid1 == 0)
  {
    for (int i = 0; i < 1000; ++i)
      puts ("b");
    return 0;
  }
  else 
  {
    pid2 = fork ();
    if (pid2 < 0) 
    {
      fprintf (stderr, "Fork Failed!\n");
      exit (1); 
    }
    else if (pid2 == 0) 
    {
      for (int i = 0; i < 1000; ++i)
        puts ("a");
      return 0;
    }
    else
    {
      for (int i = 0; i < 1000; ++i)
        puts ("c");
      return 0;
    }
  }

  return 0;
}

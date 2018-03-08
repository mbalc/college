#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "err.h"

#define NR_PROC 5

int main ()
{
  pid_t pid;
  int i;

  /* tworzenie procesów potomnych */
  for (i = 1; i <= NR_PROC; i++) {
    int pipe_dsc[2];
    if (pipe(pipe_dsc) == -1) syserr("Error in pipe\n");

    switch (pid = fork()) {
      case -1:
        syserr("Error in fork\n");

      case 0: /* proces potomny */
        printf("I am a child and my pid is %d\n", getpid());
        break;

      default: /* proces macierzysty */
        printf("I am a parent and my pid is %d\n", getpid());
        if (wait(0) == -1)
          syserr("Error in wait\n");
        return 0;
    }
  }
  return 0;
}

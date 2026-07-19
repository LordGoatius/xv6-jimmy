#include "kernel/types.h"
#include "user/user.h"

#define TICKS_PER_SECOND 10

void
sleep(char *arg)
{
  int seconds = atoi(arg);
  pause(seconds * TICKS_PER_SECOND);
}

int
main(int argc, char *argv[])
{
  if (argc == 1) {
    printf("Please pass an argument\n");
    exit(1);
  } else if (argc == 2) {
    sleep(argv[1]);
  } else {
    printf("Please pass one argument\n");
  }
  exit(0);
}

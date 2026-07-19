#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
sixfive(int fd)
{
  char buf[512];
  char curr;
  int i, num, valid;

  char *nums = "1234567890";
  char *seps = " -\r\t\n./,";

  i = 0;
  valid = true;
  while (read(fd, &curr, 1) == 1) {
    if (strchr(seps, curr)) {
      buf[i] = 0;
      if (valid) {
        num = atoi(buf);
        if (num % 5 == 0 || num % 6 == 0) {
          printf("%d\n", num);
        }
      }
      i = 0;
      valid = true;
      continue;
    }
    buf[i] = curr;
    i++;
    // Cannot be a separator
    if (!strchr(nums, curr)) {
      valid = false;
    }
  }
  buf[i] = 0;
  if (valid) {
    num = atoi(buf);
    if (num % 5 == 0 || num % 6 == 0) {
      printf("%d\n", num);
    }
  }
}

int
main(int argc, char *argv[])
{
  int i, fd;
  
  if (argc == 1) {
    printf("Please pass a file\n");
    exit(1);
  } else {
    for(i = 1; i < argc; i++){
      if((fd = open(argv[i], O_RDONLY)) < 0){
        fprintf(2, "sixfive: cannot open %s\n", argv[i]);
        exit(1);
      }
      sixfive(fd);
      close(fd);
    }
  }
  exit(0);
}

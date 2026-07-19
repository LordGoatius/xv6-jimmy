#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char*
entname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void
find(char *curr, char *name, int ex, int argc, char *argv[])
{
  char buf[512] = {}, *p;
  int fd;
  struct dirent de;
  struct stat st;

  if ((fd = open(curr, O_RDONLY)) < 0) {
    fprintf(2, "Cannot open %s", curr);
    exit(1);
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "Cannot stat %s", curr);
    exit(1);
  }

  switch (st.type) {
    case T_DEVICE:
    case T_FILE:
      if (strcmp(entname(curr), name) == 0) {
        if (ex) {
          if (fork() != 0) {
            int status;
            wait(&status);
            pause(1);
          } else {
            argv[argc] = curr;
            argv[argc + 1] = 0;
            if (exec(argv[0], argv) < 0) {
              fprintf(2, "error in exec\n");
              exit(1);
            }
          }
        } else {
          printf("%s\n", curr);
        }
      }
      break;
    case T_DIR:
      if(strlen(curr) + 1 + DIRSIZ + 1 > sizeof buf){
        fprintf(2, "path too long\n");
        break;
      }
      strcpy(buf, curr);
      p = buf+strlen(buf);
      *p++ = '/';
      while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        // Skip '.'
        if (de.inum == 0) {
          continue;
        }
        char *ent = entname(de.name);
        if (strcmp(ent, ".") == 0) {
          continue;
        }
        if (strcmp(ent, "..") == 0) {
          continue;
        }

        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0){
          fprintf(2, "can't stat %s\n", buf);
          printf("can't stat %s\n", buf);
          continue;
        }
        find(buf, name, ex, argc, argv);
      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  char *fargv[MAXARG] = {0};
  if (argc < 3) {
    printf("First arg: directory to search\n Second arg: file name to search for\n");
    fprintf(2, "Invalid arguments\n");
    exit(1);
  } else if (argc == 3) {
    find(argv[1], argv[2], false, 0, NULL);
  } else {
    // TODO: Parse argv and initalize fargv
    if (strcmp(argv[3], "-exec") == 0) {
      for (int i = 4; i < argc + 4; i++) {
        fargv[i - 4] = argv[i];
      }
      find(argv[1], argv[2], true, argc - 4, fargv);
    }
  }

  exit(0);
}

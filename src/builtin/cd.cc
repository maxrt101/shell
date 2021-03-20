#include "builtin/cd.h"
#include "shell.h"
#include "logging/log.h"

#include <errno.h>
#include <unistd.h>
#include <cstring>

int builtin::builtin_cd(void* shptr, char** args, int argc) {
  shell::Shell* sh = static_cast<shell::Shell*>(shptr);
  
  if (argc == 2) {
    int ret = chdir(args[1]);
    if (ret == -1) {
      logging::error("cd: %s", strerror(errno));
      return -1;
    }
  } else {
    logging::error("cd: expected exactly 1 argument, but got %d", argc-1);
    return -1;
  }

  return 0;
}


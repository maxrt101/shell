#include "builtin/exit.h"
#include "shell.h"
#include "logging/log.h"
#include "utils/die.h"

int builtin::builtin_exit(void* shptr, char** args, int argc) {
  shell::Shell* sh = static_cast<shell::Shell*>(shptr);
  utils::Exit();
  /** @TODO: Check for return code in args[1], and report error if there are more args */
  return 0;
}


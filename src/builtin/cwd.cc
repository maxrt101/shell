#include "builtin/cwd.h"
#include "shell.h"
#include "logging/log.h"

#include <errno.h>
#include <unistd.h>
#include <cstring>
#include <iostream>


constexpr int kCwdBufferSize = 128;


int builtin::builtin_cwd(void* shptr, char** args, int argc) {
  shell::Shell* sh = static_cast<shell::Shell*>(shptr);
  char* buffer = new char[kCwdBufferSize];

  if (getcwd(buffer, kCwdBufferSize) == NULL) {
    logging::error("cwd failed: %s", strerror(errno));
    delete [] buffer;
    return -1;
  } else {
    std::cout << buffer << std::endl;
  }

  delete [] buffer;
  return 0;
}


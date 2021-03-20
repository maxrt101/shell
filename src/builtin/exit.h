#ifndef SHELL_BUILTIN_EXIT_H_
#define SHELL_BUILTIN_EXIT_H_

#include "builtin/builtin.h"

namespace builtin {

int builtin_exit(void* shptr, char** args, int argc);

} // namespace builtin

#endif


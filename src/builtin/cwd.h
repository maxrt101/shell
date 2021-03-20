#ifndef SHELL_BUILTIN_CWD_H_
#define SHELL_BUILTIN_CWD_H_

#include "builtin/builtin.h"

namespace builtin {

int builtin_cwd(void* shptr, char** args, int argc);

} // builtin

#endif


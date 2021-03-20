#ifndef SHELL_BUILTIN_PWD_H_
#define SHELL_BUILTIN_PWD_H_

#include "builtin/builtin.h"

namespace builtin {

int builtin_pwd(void* shptr, char** args, int argc);

} // builtin

#endif


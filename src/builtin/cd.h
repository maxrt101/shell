#ifndef SHELL_BUILTIN_CD_H_
#define SHELL_BUILTIN_CD_H_

#include "builtin/builtin.h"

namespace builtin {

int builtin_cd(void* shptr, char** args, int argc);

} // namespace builtin

#endif


#ifndef SHELL_BUILTIN_BUILTIN_H_
#define SHELL_BUILTIN_BUILTIN_H_

#include <string>

#include "parser/tokens.h"


namespace builtin {

typedef int (*BuiltinFunction)(void*, char**, int);


/** RegisterBuiltin(string, BuiltinFunction) - registers builtin internally
 * `name` - builtin name
 * 'function' - builtin function
 */
void RegisterBuiltin(std::string name, BuiltinFunction function);


/** BuiltinExists(string) - checks builtin table for `name`
 * `name` - builtin name
 */
bool BuiltinExists(std::string name);


/** RunBuiltin(string, void*, Args&) - runs builtin
 * `name` - name of the builtin to run
 * `shptr` - pointer to shell::Shell instance
 * `args` - ref to vector of tokens (parsed arguments)
 */
int RunBuiltin(std::string name, void* shptr, char** args, int argc);

} // namespace builtin

#endif


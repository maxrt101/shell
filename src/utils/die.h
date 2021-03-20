#ifndef SHELL_UTILS_DIE_H_
#define SHELL_UTILS_DIE_H_

#include <cstdlib>

namespace utils {

[[noreturn]] inline void Die(int exit_code = EXIT_FAILURE) {
  exit(exit_code);
}

[[noreturn]] inline void Exit(int exit_code = EXIT_SUCCESS) {
  exit(exit_code);
}

} // namespace utils

#endif


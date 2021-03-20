#include "shell.h"

int main(int argc, char ** argv) {
  shell::Shell sh(argc, argv);

  int ret = sh.Run();

  return 0;
}


#include "builtin/builtin.h"

#include <map>

static std::map<std::string, builtin::BuiltinFunction> builtins;


void builtin::RegisterBuiltin(std::string name, builtin::BuiltinFunction function) {
  builtins[name] = function;
}


bool builtin::BuiltinExists(std::string name) {
  return builtins.find(name) != builtins.end();
}


int builtin::RunBuiltin(std::string name, void* shptr, char** args) {
  BuiltinFunction function = builtins.at(name);
  return function(shptr, args);
}


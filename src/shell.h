#ifndef SHELL_SHELL_H_
#define SHELL_SHELL_H_

#include <map>
#include <string>
#include <vector>

#include "parser/tokens.h"

namespace shell {

struct SimpleCommand {
  char** args = nullptr;
  int size = 0;

  SimpleCommand(const SimpleCommand& rhs);
  SimpleCommand(std::vector<parser::Token>::iterator _beg, std::vector<parser::Token>::iterator _end);
  ~SimpleCommand();
};

struct Command {
  std::vector<SimpleCommand> commands;
  std::string out_file;
  std::string err_file;
  std::string in_file;
  bool background = false;

  int Execute();
};

class Shell {
 public:
  /* Takes and parses command line arguments */
  Shell(int argc, char ** argv);

  /* Starts the shell, returns return code */
  int Run();

  /** SetVar(string, string) - sets variable
   * Returns false, if value existed, true - if created
   * `key` - variable name
   * `val` - variable value
   */
  bool SetVar(std::string key, std::string val);

  /** GetVar(string) - returns variable value
   * Returns empty string, if variable does not exist
   * `key` - variable name
   */
  std::string GetVar(std::string key) const;

  /** VarExists(string) - returns true if variable exists, false otherwise
   * `key` - variable name
   */
  bool VarExists(std::string key) const;

  /** SetVar(string, string) - sets alias
   * Returns false, if value existed, true - if created
   * `key` - valias name
   * `val` - alias value
   */
  bool SetAlias(std::string key, std::string val);

  /** GetAlias(string) - returns alias value
   * Returns empty string, if alias does not exist
   * `key` - alias name
   */
  std::string GetAlias(std::string key) const;

  /** AliasExists(string) - returns true if alias exists, false otherwise
   * `key` - alias name
   */
  bool AliasExists(std::string key) const;

  /** GetPrompt() - returns prompt
   * If prompt is not defines, returns default prompt
   */
  std::string GetPrompt() const;

  /** Stop() - sets running_ to false */
  void Stop();

 private:
  /* Parses line input */
  void ParseLine(std::string input);

 private:
  bool running_ = false;
  std::map<std::string, std::string> variables_;
  std::map<std::string, std::string> aliases_;
};

} // namespace shell

#endif


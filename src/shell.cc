#include "shell.h"

#include <vector>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iterator>
#include <iostream>
#include <streambuf>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <readline/readline.h>
#include <readline/history.h>

#include "logging/log.h"
#include "parser/scanner.h"
#include "utils/die.h"

/* Builtins*/
#include "builtin/exit.h"
#include "builtin/pwd.h"
#include "builtin/cd.h"


constexpr auto kDefaultPrompt = "$ ";

shell::SimpleCommand::SimpleCommand(const SimpleCommand& rhs) {
  size = rhs.size;
  args = new char*[size+1];
  for (int i = 0; i < size; i++) {
    int arg_size = strlen(rhs.args[i]);
    args[i] = new char[arg_size+1];
    strcpy(args[i], rhs.args[i]);
    args[i][arg_size] = '\0';
  }
  args[size] = NULL;
}

shell::SimpleCommand::SimpleCommand(std::vector<parser::Token>::iterator _beg, std::vector<parser::Token>::iterator _end) {
  int args_size = std::distance(_beg, _end);
  args = new char*[args_size+1];
  size = 0;
  while (_beg != _end) {
    args[size] = new char[_beg->str.size()+1];
    strncpy(args[size], _beg->str.c_str(), _beg->str.size());
    args[size][_beg->str.size()] = '\0';
    size++;
    _beg++;
  }
  args[args_size] = NULL;
}

shell::SimpleCommand::~SimpleCommand() {
  for (int i = 0; i < size; i++) {
    delete args[i];
  }
  delete args;
  args = nullptr;
}


int shell::Command::Execute() {
  int original_in = dup(STDIN_FILENO);
  int original_out = dup(STDOUT_FILENO);

  int fdin;
  if (in_file != "") {
    fdin = open(in_file.c_str(), O_RDONLY);
    if (fdin == -1) {
      logging::error("open() failed: %s", strerror(errno));
      fdin = dup(original_in);
    }
  } else {
    fdin = dup(original_in);
  }

  int ret, fdout;
  for (auto& cmd : commands) {
    std::string cmd_name = std::string(cmd.args[0]);
    if (builtin::BuiltinExists(cmd_name)) {
      builtin::RunBuiltin(cmd_name, (void*)this, cmd.args, cmd.size);
      continue;
    }
    
#ifdef _DEBUG
    std::cout << "cmd(" << cmd.size << ") [";

    for (int i = 0; i < cmd.size; i++) std::cout << "'" << cmd.args[i] << "', ";
    std::cout << "]" << std::endl;
#endif

    dup2(fdin, 0);
    close(fdin);

    if (&cmd == &(commands.back())) {
      if (out_file != "") {
        fdout = open(out_file.c_str(), O_WRONLY | O_CREAT, 0644);
        if (fdout < 0) {
          logging::error("open() failed: %s", strerror(errno));
        }
      } else {
        fdout = dup(original_out);
      }
    } else {
      int pipefds[2];
      pipe(pipefds);
      fdout = pipefds[1];
      fdin = pipefds[0];
    }

    dup2(fdout, 1);
    close(fdout);

    ret = fork();
    if (ret == 0) { // child
      execvp(cmd.args[0], cmd.args);
      logging::error("execvp() failed: %s", strerror(errno));
      exit(EXIT_FAILURE);
    } else if (ret < 0) {
      logging::error("fork() failed: %s", strerror(errno));
      return -1;
    } else if (ret != 0) {}
  }
  
  dup2(original_in, STDIN_FILENO);
  dup2(original_out, STDOUT_FILENO);
  close(original_in);
  close(original_out);

  int status = 0;
  if (!background)
    waitpid(ret, &status, 0);
  return status;
}


shell::Shell::Shell(int argc, char ** argv) {
  builtin::RegisterBuiltin("exit", builtin::builtin_exit);
  builtin::RegisterBuiltin("pwd", builtin::builtin_pwd);
  builtin::RegisterBuiltin("cd", builtin::builtin_cd);
}


bool shell::Shell::SetVar(std::string key, std::string val) {
  bool exists = VarExists(key);
  variables_[key] = val;
  return exists;
}


std::string shell::Shell::GetVar(std::string key) const {
  if (VarExists(key)) {
    return variables_.at(key);
  }
  return "";
}


bool shell::Shell::VarExists(std::string key) const {
  return variables_.find(key) != variables_.end();
}


std::string shell::Shell::GetPrompt() const {
  if (VarExists("PROMPT")) {
    return GetVar("PROMPT") + " ";
  }
  return kDefaultPrompt;
}


void shell::Shell::Stop() {
  running_ = false;
}


int shell::Shell::Run() {
  if (running_) return -1;
  running_ = true;

  char* buffer = NULL;

  std::string prompt = GetPrompt();
  while (running_) {
    buffer = readline(prompt.c_str());
    if (buffer == NULL) break;
    if (strlen(buffer) > 0) {
      add_history(buffer);
      logging::debug("IN: '%s'", buffer);
      ParseLine(std::string(buffer));
    }

    free(buffer);
  }
  return 0;
}


void shell::Shell::ParseLine(std::string input) {
  std::vector<parser::Token> tokens = parser::SplitLine(input);
  if (tokens.size() == 0) return;

  // std::cout << "CMD:" << tokens.size() << "\n";
  // for (auto& token : tokens) std::cout << "\t" << static_cast<int>(token.type) << " " << token.str << "\n";

  if (tokens.back().type == parser::TOKEN_ERROR) {
    logging::error("Error parsing command '%s': %s", input.c_str(), tokens.back().str.data());
    return;
  }

  if (tokens.size() > 1) {
    if (tokens[1].type == parser::TOKEN_EQUALS) {
      std::string value;
      for (int i = 2; i < tokens.size(); i++) {
        value += tokens[i].str;
      }
      SetVar(tokens[0].str, value);
      return;
    }
  }

  Command cmd;
  std::vector<parser::Token>::iterator beg = tokens.begin();

  /** In this for loop you can see a lot of erasing, so I will try to explain that part.
   * The erasing is done prevent shell utility tokens, like '>' or '$', appearing in actual command
   * If we don't erase theese tokens, they will appear in commad arguments, because arguments
   * are constructed from 2 iterators(this is basically a range), and that iterators don't skip 
   * utility tokens by default, they just iterate over token vector.
   */

  for (auto itr = tokens.begin(); itr != tokens.end(); itr++) {
    if (itr->type == parser::TOKEN_PIPE) {
      cmd.commands.push_back(SimpleCommand(beg, itr));
      beg = itr+1;
    } else if (itr->type == parser::TOKEN_AMPERSAND) {
      cmd.background = true;
      tokens.erase(itr--); // Erase '&' from token vector
    } else if (itr->type == parser::TOKEN_GREATER) {
      cmd.out_file = (itr+1)->str;
      // Erase '>' and file name from token vector
      auto here = itr--;
      tokens.erase(here, here+2); 
    } else if (itr->type == parser::TOKEN_LESS) {
      cmd.in_file = (itr+1)->str;
      // Erase '<' and file name from token vector
      auto here = itr--;
      tokens.erase(here, here+2); 
    } else if (itr->type == parser::TOKEN_DOLLAR) {
      auto next = itr+1; 
      itr->str = GetVar(next->str);
      tokens.erase(next); // Erase '$' from token vector
    } else if (itr->type == parser::TOKEN_NESTED) {
      /* FIXME: This clearly doesn't work as it should have,
       * mainly because writing to file creates otherwise unintended newlines
       * output should be redirected in some other way
       * TODO: Consider pipe */
      std::string tmp_file_name = "/tmp/shell_" + std::to_string(getpid()); // unique file name using PID
      std::string nested_cmd = itr->str;
      nested_cmd += " > " + tmp_file_name;
      ParseLine(nested_cmd);
      std::ifstream tmp_file(tmp_file_name);
      std::string nested_result((std::istreambuf_iterator<char>(tmp_file)),
                                 std::istreambuf_iterator<char>());
      itr->str = nested_result;             // TODO: maybe std::move()?
      itr->type = parser::TOKEN_IDENTIFIER; // Set another type to prevent infinite loop
    }
  }

  cmd.commands.push_back(SimpleCommand(beg, tokens.end()));
  cmd.Execute();
}


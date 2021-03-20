# shell
Unix shell written in c++

## Overview
This is simple implementation of unix command proccessor.

### Features
 - [X] Can run commands in PATH
 - [X] Pipes (cmd1 | cmd2)
 - [X] Output redirection (cmd > file)
 - [X] Input redirection (cmd < file)
 - [X] Support for builtins
 - [X] Variables support
 - [ ] Aliases support
 - [ ] Customizable prompt
 - [ ] Start command in bg (cmd &)
 - [ ] Jobs support
 - [ ] Functions support

### Builtins
 - `exit` - exits the shell
 - `cd` - change directory
 - `pwd` - current working directory
 - **PLANNED** `kill`
 - **PLANNED** `print`
 - **PLANNED** `echo`
 - **PLANNED** `alias`
 - **PLANNED** `declare` 

## Instalation
 - Clone/Download repo
 - Go into repo folder
 - Type `make`
This will produce `shell` executable, which you can run with `./shell`


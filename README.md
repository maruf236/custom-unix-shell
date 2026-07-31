# Custom Unix Shell
A custom Unix shell built in C++ using POSIX system calls as part of an Operating Systems course project.

**Team:** Commander

## Overview
This project implements a simplified Unix shell that can execute commands, manage processes, and demonstrate core Operating System concepts such as process creation, inter-process communication, and file I/O.

## Features

### Completed
- [x] Basic shell prompt
- [x] Command parsing
- [x] External command execution (`fork()` + `execvp()`)
- [x] Built-in commands (`cd`, `pwd`, `exit`)
- [x] Single pipe (`|`)
- [x] Input redirection (`<`)
- [x] Output redirection (`>`)
- [x] Background processes (`&`)
- [x] Command history
- [x] Alias management (`alias`, `unalias`)
- [x] Zombie process handling (`SIGCHLD` handler)

## Technologies
- C++ (C++17)
- POSIX System Calls
- Linux (Ubuntu)
- GCC/G++

## POSIX System Calls Used
- `fork()`
- `execvp()`
- `wait()` / `waitpid()`
- `pipe()`
- `dup2()`
- `open()`
- `close()`
- `chdir()`
- `getcwd()`
- `sigaction()` (signal handling for `SIGCHLD`)

## Project Structure
```
CustomShell/
├── src/
│   ├── main.cpp         # Shell prompt loop
│   ├── parser.cpp       # Turns input text into commands
│   ├── executor.cpp     # fork/exec, pipes, redirection
│   ├── builtins.cpp     # cd, pwd, exit, history, alias
│   └── signals.cpp      # Zombie process cleanup
├── include/
│   └── shell.h          # Shared structs and function declarations
├── Makefile
└── README.md
```

## Build
```bash
make
```

## Run
```bash
./shell
```

## Usage Examples
```
commander:/home/user$ pwd
commander:/home/user$ ls -l | grep .cpp
commander:/home/user$ echo hello > out.txt
commander:/home/user$ cat < out.txt
commander:/home/user$ sleep 5 &
commander:/home/user$ alias ll=ls -l
commander:/home/user$ ll
commander:/home/user$ history
commander:/home/user$ exit
```

## Learning Objectives
- Process creation and management
- Parent and child processes
- Process synchronization
- Pipes and inter-process communication (IPC)
- File descriptor manipulation
- Input/output redirection
- Signal handling
- Unix system programming

## Author
Ali Ahsan Maruf

# Custom Unix Shell

A custom Unix shell built in C++ using POSIX system calls as part of an Operating Systems course project.

## Overview

This project aims to implement a simplified Unix shell that can execute commands, manage processes, and demonstrate core Operating System concepts such as process creation, inter-process communication, and file I/O.

## Features

### Completed
- [ ] Basic shell prompt
- [ ] Command parsing
- [ ] External command execution (`fork()` + `execvp()`)
- [ ] Built-in commands (`cd`, `pwd`, `exit`)
- [ ] Single pipe (`|`)
- [ ] Input redirection (`<`)
- [ ] Output redirection (`>`)
- [ ] Background processes (`&`)
- [ ] Command history
- [ ] Alias management
- [ ] Zombie process handling

## Technologies

- C++
- POSIX System Calls
- Linux (Ubuntu/WSL)
- GCC/G++

## POSIX System Calls Used

- `fork()`
- `execvp()`
- `wait()`
- `waitpid()`
- `pipe()`
- `dup2()`
- `open()`
- `close()`
- `chdir()`
- `getcwd()`
- `signal()`

## Project Structure

```
CustomShell/
├── src/
├── include/
├── Makefile
├── README.md
```

## Build

```bash
make
```

## Run

```bash
./shell
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

**Ali Ahsan Maruf**
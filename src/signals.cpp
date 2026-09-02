// signals.cpp
// Handles finished background processes.
// Without this, finished background processes become zombies.
// SIGCHLD is sent by the OS when a child process finishes.
// The handler uses waitpid() to clean it immediately.
#include "shell.h"
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <cstdio>
using namespace std;
// Runs automatically when a child process finishes
void signalHandler(int signal)
{
    // Avoid unused parameter warning
    (void)signal;
    int status;
    pid_t pid;
    // Check all finished child processes
    // WNOHANG prevents the shell from waiting
    while(
        (pid = waitpid(-1, &status, WNOHANG)) > 0
    )
    {
        char message[64];
        int length = snprintf(
            message,
            sizeof(message),
            "\nBackground process %d finished\n",
            pid
        );
        if(length > 0)
        {
            write(
                STDOUT_FILENO,
                message,
                length
            );
        }
    }
}
void setup_signal_handlers()
{
    struct sigaction action;
    action.sa_handler = signalHandler;
    sigemptyset(
        &action.sa_mask
    );
    action.sa_flags = SA_RESTART;
    if(
        sigaction(
            SIGCHLD,
            &action,
            nullptr
        ) == -1
    )
    {
        perror(
            "Signal setup failed"
        );
    }
}
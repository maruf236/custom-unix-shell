// executor.cpp
//
// Executes external commands using fork() and execvp()
// Handles pipes and input/output redirection.


#include "shell.h"

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <cstdlib>

using namespace std;



// Convert vector<string> into char* array for execvp()

vector<char*> createArguments(Command &cmd)
{
    vector<char*> args;


    for(string &word : cmd.args)
    {
        args.push_back(&word[0]);
    }


    args.push_back(nullptr);

    return args;
}



// Handle input and output redirection

void redirect(Command &cmd)
{

    // Input redirection <

    if(!cmd.input_file.empty())
    {
        int file = open(
            cmd.input_file.c_str(),
            O_RDONLY
        );


        if(file == -1)
        {
            perror("Input file error");
            exit(1);
        }


        dup2(file, STDIN_FILENO);

        close(file);
    }



    // Output redirection >

    if(!cmd.output_file.empty())
    {
        int file = open(
            cmd.output_file.c_str(),
            O_WRONLY | O_CREAT | O_TRUNC,
            0644
        );


        if(file == -1)
        {
            perror("Output file error");
            exit(1);
        }


        dup2(file, STDOUT_FILENO);

        close(file);
    }

}



// Execute normal command

void runCommand(Command &cmd)
{

    if(cmd.args.empty())
        return;


    pid_t pid = fork();



    // Child process

    if(pid == 0)
    {

        redirect(cmd);


        vector<char*> args = createArguments(cmd);


        execvp(
            args[0],
            args.data()
        );


        perror("Command failed");

        exit(1);
    }



    // Parent process

    else if(pid > 0)
    {

        if(cmd.background)
        {
            cout
            << "Background process started: "
            << pid
            << endl;
        }

        else
        {
            waitpid(
                pid,
                nullptr,
                0
            );
        }

    }


    else
    {
        perror("Fork failed");
    }

}



// Execute command1 | command2

void runPipe(Command &first, Command &second)
{

    int pipeFile[2];


    if(pipe(pipeFile) == -1)
    {
        perror("Pipe failed");
        return;
    }



    // First command

    pid_t firstPID = fork();



    if(firstPID == 0)
    {

        dup2(
            pipeFile[1],
            STDOUT_FILENO
        );


        close(pipeFile[0]);

        close(pipeFile[1]);



        vector<char*> args =
            createArguments(first);



        execvp(
            args[0],
            args.data()
        );


        perror("First command failed");

        exit(1);

    }



    // Second command

    pid_t secondPID = fork();



    if(secondPID == 0)
    {

        dup2(
            pipeFile[0],
            STDIN_FILENO
        );


        close(pipeFile[0]);

        close(pipeFile[1]);



        vector<char*> args =
            createArguments(second);



        execvp(
            args[0],
            args.data()
        );


        perror("Second command failed");

        exit(1);

    }



    // Parent closes pipe

    close(pipeFile[0]);

    close(pipeFile[1]);



    waitpid(
        firstPID,
        nullptr,
        0
    );


    waitpid(
        secondPID,
        nullptr,
        0
    );

}



// Main executor

bool execute_pipeline(Pipeline &pipeline)
{

    // Single command

    if(!pipeline.has_pipe)
    {

        if(pipeline.cmd1.args.empty())
            return false;



        // Built-in commands

        if(is_builtin(pipeline.cmd1.args[0]))
        {
            return run_builtin(pipeline.cmd1);
        }



        runCommand(
            pipeline.cmd1
        );

    }



    // Pipe command

    else
    {

        runPipe(
            pipeline.cmd1,
            pipeline.cmd2
        );

    }



    return false;
}
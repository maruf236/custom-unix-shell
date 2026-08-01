// main.cpp
//
// The shell's main loop:
//   1. Print a prompt
//   2. Read a line from the user
//   3. Parse it
//   4. Execute it
//   5. Repeat until the user types "exit"

#include "shell.h"
#include <iostream>
#include <unistd.h>
#include <limits.h>

using namespace std;


int main()
{
    // Start signal handling
    // This helps to remove zombie processes
    setup_signal_handlers();


    string command;


    while(true)
    {
        // 1. Show shell prompt
        char currentPath[PATH_MAX];

        if(getcwd(currentPath, sizeof(currentPath)))
        {
            cout << "commander:" << currentPath << "$ ";
        }
        else
        {
            cout << "commander$ ";
        }


        // 2. Take user input

        getline(cin, command);

        // Ctrl + D handling - for exit 
        if(cin.eof())
        {
            cout << "\nGoodbye!\n";
            break;
        }

        // Empty command
        if(command == "")
        {
            continue;
        }
        // 3. Store command history

        add_to_history(command);

        // 4. Convert text into command
        Pipeline pipeline;

        pipeline = parse_line(command);
        // 5. Execute command

        bool exitShell;
        exitShell = execute_pipeline(pipeline);
        // 6. Exit shell

        if(exitShell)
        {
            break;
        }
    }
    return 0;
}
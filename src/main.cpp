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
        char currentPath[PATH_MAX];
        if(getcwd(currentPath, sizeof(currentPath)))
        {
            cout
            << "commander:"
            << currentPath
            << "$ ";
        }

        else
        {
            cout << "commander$ ";
        }
        getline(cin, command);
        if(cin.eof())
        {
            cout << "\nGoodbye!\n";
            break;
        }
        if(command.empty())
        {
            continue;
        }
        add_to_history(command);
        Pipeline pipeline;
        pipeline = parse_line(command);
        bool exitShell;
        exitShell = execute_pipeline(pipeline);
        if(exitShell)
        {
            break;
        }
    }
    return 0;
}
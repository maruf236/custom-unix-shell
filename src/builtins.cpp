// builtins.cpp
//
// Built-in commands run inside the shell itself.
// Commands like cd must not run in a child process,
// otherwise the shell directory will not change.

#include "shell.h"
#include <iostream>
#include <unistd.h>
#include <limits.h>
#include <cstdlib>

using namespace std;
// Store history and aliases
vector<string> g_history;
map<string, string> g_aliases;

// Check if command is a built-in command
bool is_builtin(string command)
{
    return command == "cd" ||
           command == "pwd" ||
           command == "exit" ||
           command == "history" ||
           command == "alias" ||
           command == "unalias";
}
// Change current directory
void changeDirectory(Command &cmd)
{
    string path;

    // If no directory is given, go to home directory
    if(cmd.args.size() < 2)
    {
        char *home = getenv("HOME");

        if(home == nullptr)
        {
            cout << "HOME not found" << endl;
            return;
        }

        path = home;
    }
    else
    {
        path = cmd.args[1];
    }

    if(chdir(path.c_str()) != 0)
    {
        perror("cd failed");
    }
}
// Print current directory
void printDirectory()
{
    char path[PATH_MAX];

    if(getcwd(path, sizeof(path)))
    {
        cout << path << endl;
    }
    else
    {
        perror("pwd failed");
    }
}
// Add command to history
void add_to_history(const string &command)
{
    if(command.empty())
        return;
    g_history.push_back(command);

    // Remove oldest command if history becomes too large
    if(g_history.size() > HISTORY_SIZE)
    {
        g_history.erase(g_history.begin());
    }
}
// Display command history
void showHistory()
{
    for(int i = 0; i < g_history.size(); i++)
    {
        cout << i + 1 << " "
             << g_history[i]
             << endl;
    }
}
// Create or update an alias
void createAlias(string name, string value)
{
    g_aliases[name] = value;
}
// Delete an alias
void removeAlias(string name)
{
    g_aliases.erase(name);
}
// Display all aliases
void showAliases()
{
    for(auto alias : g_aliases)
    {
        cout << "alias "
             << alias.first
             << "="
             << alias.second
             << endl;
    }
}
// Replace alias with original command
string expand_alias(const string &command)
{
    if(g_aliases.find(command) != g_aliases.end())
    {
        return g_aliases[command];
    }
    return command;
}
// Handle alias command
// Example:
// alias ll=ls -l
void handleAlias(Command &cmd)
{
    if(cmd.args.size() < 2)
    {
        showAliases();
        return;
    }
    string data;

    // Join all arguments after alias
    for(int i = 1; i < cmd.args.size(); i++)
    {
        if(i > 1)
            data += " ";

        data += cmd.args[i];
    }
    int position = data.find('=');
    if(position == string::npos)
    {
        cout << "Wrong alias format" << endl;
        return;
    }
    string name = data.substr(0, position);
    string value = data.substr(position + 1);
    createAlias(name, value);
}
// Remove alias
void handleUnalias(Command &cmd)
{
    if(cmd.args.size() < 2)
    {
        cout << "unalias requires name" << endl;
        return;
    }


    removeAlias(cmd.args[1]);
}
// Run built-in command
// Return true when shell needs to exit
bool run_builtin(Command &cmd)
{
    string command = cmd.args[0];


    if(command == "cd")
    {
        changeDirectory(cmd);
    }
    else if(command == "pwd")
    {
        printDirectory();
    }
    else if(command == "exit")
    {
        cout << "Goodbye!" << endl;
        return true;
    }
    else if(command == "history")
    {
        showHistory();
    }
    else if(command == "alias")
    {
        handleAlias(cmd);
    }
    else if(command == "unalias")
    {
        handleUnalias(cmd);
    }


    return false;
}
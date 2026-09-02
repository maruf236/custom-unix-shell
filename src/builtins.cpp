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


// Check if command is a built-in command or not 

bool is_builtin(const string &cmd_name)
{
    return cmd_name == "cd" ||
           cmd_name == "pwd" ||
           cmd_name == "exit" ||
           cmd_name == "history" ||
           cmd_name == "alias" ||
           cmd_name == "unalias";
}

void changeDirectory(Command &cmd)
{
    string path;
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

void add_to_history(const string &command)
{
    if(command.empty())
        return;
    g_history.push_back(command);
    if(g_history.size() > HISTORY_SIZE)
    {
        g_history.erase(g_history.begin());
    }
}

void showHistory()
{
    for(size_t i = 0; i < g_history.size(); i++)
    {
        cout << i + 1
             << " "
             << g_history[i]
             << endl;
    }
}

void createAlias(string name, string value)
{
    g_aliases[name] = value;
}

void removeAlias(string name)
{
    g_aliases.erase(name);
}

void showAliases()
{
    for(auto &alias : g_aliases)
    {
        cout << "alias "
             << alias.first
             << "="
             << alias.second
             << endl;
    }
}

string expand_alias(const string &command)
{
    if(g_aliases.find(command) != g_aliases.end())
    {
        return g_aliases[command];
    }

    return command;
}


// Handle alias command

void handleAlias(Command &cmd)
{
    if(cmd.args.size() < 2)
    {
        showAliases();
        return;
    }


    string data;

    for(size_t i = 1; i < cmd.args.size(); i++)
    {
        if(i > 1)
            data += " ";

        data += cmd.args[i];
    }


    size_t position = data.find('=');


    if(position == string::npos)
    {
        cout << "Wrong alias format" << endl;
        return;
    }


    string name = data.substr(0, position);

    string value = data.substr(position + 1);


    createAlias(name, value);
}

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
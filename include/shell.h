#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#define HISTORY_SIZE 100


// Stores one command
// Example:
// ls -l > file.txt &

struct Command
{
    vector<string> args;

    // File used for input redirection (<)
    string input_file;

    // File used for output redirection (>)
    string output_file;

    // True when command runs in background (&)
    bool background = false;
};


// Stores complete user input
// Example:
// ls | grep txt

struct Pipeline
{
    Command cmd1;
    Command cmd2;

    // True when pipe (|) exists
    bool has_pipe = false;
};


// Parser functions

vector<string> tokenize(const string &line);

Pipeline parse_line(const string &line);


// Executor functions

bool execute_pipeline(Pipeline &pipeline);

void run_single_command(Command &cmd);

void run_piped_commands(Command &cmd1, Command &cmd2, bool background);


// Built-in command functions

bool is_builtin(const string &cmd_name);

bool run_builtin(Command &cmd);

void add_to_history(const string &line);

void print_history();

void set_alias(const string &name, const string &value);

void remove_alias(const string &name);

void print_aliases();

string expand_alias(const string &cmd_name);


// Signal handling

void setup_signal_handlers();


// Shared variables used by multiple files

extern vector<string> g_history;

extern map<string, string> g_aliases;


#endif
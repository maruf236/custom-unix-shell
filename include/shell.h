#ifndef SHELL_H
#define SHELL_H

#include <string>
#include <vector>
#include <map>

using namespace std;

#define HISTORY_SIZE 100

// Stores one command

struct Command
{
    vector<string> args;

    string input_file;

    string output_file;

    bool background = false;
};
// Stores complete user input

struct Pipeline
{
    Command cmd1;
    Command cmd2;

    bool has_pipe = false;
};

vector<string> tokenize(const string &line); // perser

Pipeline parse_line(const string &line);

bool execute_pipeline(Pipeline &pipeline);

void run_single_command(Command &cmd);

void run_piped_commands(Command &cmd1, Command &cmd2, bool background);

bool is_builtin(const string &cmd_name);

bool run_builtin(Command &cmd);

void add_to_history(const string &line);

void print_history();

void set_alias(const string &name, const string &value);

void remove_alias(const string &name);

void print_aliases();

string expand_alias(const string &cmd_name);

void setup_signal_handlers();

extern vector<string> g_history;

extern map<string, string> g_aliases;


#endif
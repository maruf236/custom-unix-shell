#include "shell.h"
#include <sstream>
using namespace std;
// Convert full command line into separate words
// Example:
// "ls -l | grep cpp"
// becomes:
// ["ls", "-l", "|", "grep", "cpp"]
vector<string> tokenize(const string &line)
{
    vector<string> words;
    string word;
    stringstream ss(line);
    while(ss >> word)
    {
        words.push_back(word);
    }
    return words;
}

// Convert one command part into Command structure
//
// Example:
//
// ls -l > file.txt
//
// becomes:
//
// args:
//      ls
//      -l
//
// output_file:
//      file.txt
Command parse_command(vector<string> words)
{
    Command cmd;
    for(int i=0; i<words.size(); i++)
    {
       // Input redirection
        if(words[i] == "<")
        {
            if(i+1 < words.size())
            {
                cmd.input_file = words[i+1];
                i++;
            }
        }
        // Output redirection
        else if(words[i] == ">")
        {
            if(i+1 < words.size())
            {
                cmd.output_file = words[i+1];
                i++;
            }
        }
        // Normal argument
        else
        {
            cmd.args.push_back(words[i]);
        }
    }
    return cmd;
}
Pipeline parse_line(string line)
{
    Pipeline pipeline;
    // Step 1:
    // Convert string into words
    vector<string> words = tokenize(line);
    if(words.empty())
    {
        return pipeline;
    }
    // Step 2:
    // Check background process (&)
    bool background = false;
    if(words.back()=="&")
    {
        background = true;
        words.pop_back();
    }
    // Step 3:
    // Check alias
    string aliasCommand = expand_alias(words[0]);
    if(aliasCommand != words[0])
    {
        vector<string> aliasWords = tokenize(aliasCommand);
        words.erase(words.begin());
        words.insert(
            words.begin(),
            aliasWords.begin(),
            aliasWords.end()
        );
    }
    // Step 4:
    // Search for pipe symbol |
    int pipePosition = -1;
    for(int i=0; i<words.size(); i++)
    {
        if(words[i]=="|")
        {
            pipePosition=i;
            break;
        }
    }
    // No pipe
    if(pipePosition == -1)
    {
        pipeline.has_pipe=false;
        pipeline.cmd1 =
            parse_command(words);
        pipeline.cmd1.background =
            background;
    }
    // Pipe exists
    else
    {
        vector<string> firstCommand(
            words.begin(),
            words.begin()+pipePosition
        );
        vector<string> secondCommand(
            words.begin()+pipePosition+1,
            words.end()
        );
        pipeline.has_pipe=true;
        pipeline.cmd1 =
            parse_command(firstCommand);
        pipeline.cmd2 =
            parse_command(secondCommand);
        // Background belongs to final command
        pipeline.cmd2.background =
            background;
    }

    return pipeline;
}
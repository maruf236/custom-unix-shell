// parser.cpp
// Converts user input string into Command and Pipeline structures.
#include "shell.h"
#include <sstream>
using namespace std;



// Convert full command line into separate words

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

// ls -l > file.txt
// args:
//      ls
//      -l
// output_file:
//      file.txt


Command parse_command(vector<string> words)
{
    Command cmd;
    for(size_t i = 0; i < words.size(); i++)
    {
        if(words[i] == "<")
        {
            if(i + 1 < words.size())
            {
                cmd.input_file = words[i + 1];
                i++;
            }
        }
        else if(words[i] == ">")
        {
            if(i + 1 < words.size())
            {
                cmd.output_file = words[i + 1];
                i++;
            }
        }
        else
        {
            cmd.args.push_back(words[i]);
        }
    }

    return cmd;
}

Pipeline parse_line(const string &line)
{
    Pipeline pipeline;
    vector<string> words = tokenize(line);
    if(words.empty())
    {
        return pipeline;
    }
    bool background = false;
    if(words.back() == "&")
    {
        background = true;
        words.pop_back();
    }
    string aliasCommand =
        expand_alias(words[0]);
    if(aliasCommand != words[0])
    {
        vector<string> aliasWords =
            tokenize(aliasCommand);
        words.erase(
            words.begin()
        );
        words.insert(
            words.begin(),
            aliasWords.begin(),
            aliasWords.end()
        );

    }
    int pipePosition = -1;
    for(size_t i = 0; i < words.size(); i++)
    {
        if(words[i] == "|")
        {
            pipePosition = i;
            break;
        }
    }
    if(pipePosition == -1)
    {
        pipeline.has_pipe = false;
        pipeline.cmd1 =
            parse_command(words);
        pipeline.cmd1.background =
            background;
    }
    else
    {
        vector<string> firstCommand(
            words.begin(),
            words.begin() + pipePosition
        );
        vector<string> secondCommand(
            words.begin() + pipePosition + 1,
            words.end()
        );
        pipeline.has_pipe = true;
        pipeline.cmd1 =
            parse_command(firstCommand);
        pipeline.cmd2 =
            parse_command(secondCommand);
        pipeline.cmd2.background =
            background;
    }
    return pipeline;
}
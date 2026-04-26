#include "parser.h"
#include <cstring>
#include <iostream>

std::vector<char*> read_input(std::string& input) {
    std::vector<char*> args;
    char *token = strtok(input.data(), " ");
    while (token != nullptr) {
        args.push_back(token);
        token = strtok(nullptr, " ");
    }
    args.push_back(nullptr);
    return args;
}

std::vector<std::string> split_pipes(const std::string& input) {
    std::vector<std::string> commands;
    std::string current;

    for (char c : input) {
        if (c == '|') {
            if (!current.empty()) {
                commands.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }

    if (!current.empty()) {
        commands.push_back(current);
    }
    return commands;
}


Redirection parse_redirections(std::string& input) {
    Redirection commands;
    std::string current;

    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] == '>') {
            if (i + 1 < input.size() && input[i + 1] == '>') {
                commands.append = true;
                ++i;
            } 
            ++i;
            while (i < input.size() && input[i] == ' ') 
                ++i;
            
            size_t start = i;
            while (i < input.size() && input[i] != ' ' && input[i] != '>' && input[i] != '<')
                ++i;
            commands.output_file = input.substr(start, i - start);
        } else if (input[i] == '<') {
            commands.append = false;
            ++i;
            while (i < input.size() && input[i] == ' ') 
                ++i;

            size_t start = i;
            while (i < input.size() && input[i] != ' ' && input[i] != '>' && input[i] != '<')
                ++i;

            commands.input_file = input.substr(start, i - start);
        } else {
            current += input[i];
        }
    }
    input = current;
    return commands;
}

ParsedPipeline parse_pipeline(const std::string& input) {
    ParsedPipeline result;
    result.splits = split_pipes(input);
    
    for (auto& split : result.splits) {
        Redirection redirect = parse_redirections(split);
        result.redirections.push_back(redirect);
        result.commands.push_back(read_input(split));
    }
    
    return result;
}

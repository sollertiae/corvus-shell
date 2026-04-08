#include "parser.h"
#include <cstring>

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

ParsedPipeline parse_pipeline(const std::string& input) {
    ParsedPipeline result;
    result.splits = split_pipes(input);
    
    for (auto& split : result.splits) {
        result.commands.push_back(read_input(split));
    }
    
    return result;
}
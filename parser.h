#pragma once
#include <vector>
#include <string>

struct Redirection {
    std::string input_file;
    std::string output_file;
    bool append = false;
};

struct ParsedPipeline {
    std::vector<std::string> splits;
    std::vector<std::vector<char*>> commands;
    std::vector<Redirection> redirections;
};

std::vector<char*> read_input(std::string& input);
std::vector<std::string> split_pipes(const std::string& input);
ParsedPipeline parse_pipeline(const std::string& input);


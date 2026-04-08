#pragma once
#include <vector>
#include <string>
struct ParsedPipeline {
    std::vector<std::string> splits;
    std::vector<std::vector<char*>> commands;
};

std::vector<char*> read_input(std::string& input);
std::vector<std::string> split_pipes(const std::string& input);
ParsedPipeline parse_pipeline(const std::string& input);


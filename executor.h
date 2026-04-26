#pragma once
#include <vector>
#include <unistd.h>
#include "parser.h"

void execute_command(
    std::vector<char*>& command,
    Redirection& redirections,
    int in_fd = STDIN_FILENO, 
    int out_fd = STDOUT_FILENO
);
void execute_pipeline(
    std::vector<std::vector<char*>>& commands,
    std::vector<Redirection>& redirections
);
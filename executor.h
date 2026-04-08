#pragma once
#include <vector>
#include <unistd.h>

void execute_command(std::vector<char*>& command, 
                    int in_fd = STDIN_FILENO, 
                    int out_fd = STDOUT_FILENO);
void execute_pipeline(std::vector<std::vector<char*>>& commands);
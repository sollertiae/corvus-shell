#include "executor.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <vector>

void execute_command(
    std::vector<char*>& command, 
    int in_fd, 
    int out_fd
) {
    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "[!] Error creating fork" << "\n";
    } else if (pid == 0) {
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        if (execvp(command[0], command.data()) < 0) {
            std::cerr << "[!] Error executing the command " << command[0] << "\n";
            _exit(1);
        }
    }
}

void execute_pipeline(std::vector<std::vector<char*>>& commands) {
    int command_num = commands.size();
    std::vector<std::vector<int>> fd(command_num - 1, std::vector<int>(2));
    std::vector<pid_t> pid_child(command_num);

    for (int i = 0; i < command_num - 1; ++i) {
        pipe(fd[i].data());
    }
    for (int i = 0; i < command_num; ++i) {
        pid_child[i] = fork();
        if (pid_child[i] < 0) {
            std::cerr << "[!] Error creating fork with pipes" << "\n";
        } else if (pid_child[i] == 0) {
            int in_fd = (i == 0) ? STDIN_FILENO : fd[i - 1][0];
            int out_fd = (i == command_num - 1) ? STDOUT_FILENO : fd[i][1];
            execute_command(commands[i], in_fd, out_fd);
            _exit(1);
        }
    }
    for (int i = 0; i < command_num - 1; i++) {
        close(fd[i][0]);
        close(fd[i][1]);
    }
    for (int i = 0; i < command_num; i++) {
        waitpid(pid_child[i], nullptr, 0);
    }
}
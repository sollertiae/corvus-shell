#include <cstdlib>
#include <iostream>
#include <sys/wait.h>
#include <vector>
#include <fcntl.h>
#include <unistd.h> 
#include "executor.h"

void execute_command(
    std::vector<char*>& command, 
    Redirection& redirections,
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
        if (!redirections.output_file.empty()) {
            int flags = O_WRONLY | O_CREAT | (redirections.append ? O_APPEND : O_TRUNC);
            int fd_out = open(redirections.output_file.c_str(), flags, 0644);
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);
        }
        if (!redirections.input_file.empty()) {
            int fd_in = open(redirections.input_file.c_str(), O_RDONLY);
            dup2(fd_in, STDIN_FILENO);
            close(fd_in);
        }
        if (execvp(command[0], command.data()) < 0) {
            std::cerr << "[!] Error executing the command " << command[0] << "\n";
            _exit(1);
        }
    }
}

void execute_pipeline(
    std::vector<std::vector<char*>>& commands, 
    std::vector<Redirection>& redirections
) {
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
            for (int j = 0; j < command_num - 1; ++j) {
                if (fd[j][0] != in_fd)
                    close(fd[j][0]);
                if (fd[j][1] != out_fd)
                    close(fd[j][1]);
            }
            if (!redirections[i].output_file.empty()) {
                int flags = O_WRONLY | O_CREAT | (redirections[i].append ? O_APPEND : O_TRUNC);
                int fd_out = open(redirections[i].output_file.c_str(), flags, 0644);
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
            if (!redirections[i].input_file.empty()) {
                int fd_in = open(redirections[i].input_file.c_str(), O_RDONLY);
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }

            execute_command(commands[i], redirections[i], in_fd, out_fd);
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
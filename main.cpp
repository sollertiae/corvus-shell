#include <iostream>
#include <unistd.h> //required for forks
#include <sys/wait.h> //required for wait
#include <string> 
#include <vector>
#include <map>
#include <functional>
#include <cstdlib>

int builtin_cd(std::vector<char*>& args) {
    if (args.size() < 2 || args[1] == nullptr) {
        std::cerr << "[!] cd: missing argument" << "\n";
        return -1;        
    }
    if (chdir(args[1]) < 0) {
        std::cerr << "[!] cd: " << args[1] << " not" << "\n";
        return -1;
    }
    return 0;
}
int builtin_exit(std::vector<char*>& args) {
    exit(0);
}

std::map<std::string, std::function<int(std::vector<char*>&) >> builtins_map = {
    {"cd", builtin_cd},
    {"exit", builtin_exit}
};

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

std::vector<char*> read_input(char *input) {
    std::vector<char*> args;
    char *token = strtok(input, " ");
    while (token != nullptr) {
        args.push_back(token);
        token = strtok(nullptr, " ");
    }
    args.push_back(nullptr);
    return args;
}

int main() {
    std::vector<char*> command;
    std::string input;
    pid_t pid_child, wait_result;
    int stat_loc;
    while (true) {
        std::cout << "corvus >> ";
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        std::vector<std::string> pipe_parse = split_pipes(input);
        command = read_input(pipe_parse[0].data());
        if (pipe_parse.size() > 1) {
            int fd[2];
            pid_t pid_child_p;
            std::vector<char*> command_2 = read_input(pipe_parse[1].data());
            pipe(fd);
            pid_child_p = fork();
            if (pid_child_p < 0) {
                std::cerr << "[!] Error creating fork with pipes" << "\n";
            } else if (pid_child_p == 0) {
                std::cout << "Hello from child 1\n";
                close(fd[0]);
                dup2(fd[1], STDOUT_FILENO);
                if (execvp(command[0], command.data()) < 0) {
                    std::cerr << "[!] Error executing the command " << command[0] << "\n";
                }
            } else {
                std::cout << "Hello from child 2\n";
                pid_t pid_child_p2 = fork();
                if (pid_child_p2 < 0) {
                    std::cerr << "[!] Error creating fork with pipes" << "\n";
                }  else if (pid_child_p2 == 0) {
                    close(fd[1]);
                    dup2(fd[0], STDIN_FILENO);
                    if (execvp(command_2[0], command_2.data()) < 0) {
                        std::cerr << "[!] Error executing the command " << command[0] << "\n";
                    }
                } else {
                    close(fd[0]);
                    close(fd[1]);
                    waitpid(pid_child_p, nullptr, 0);
                    waitpid(pid_child_p2, nullptr, 0);
                }
            }

        } else {
            
            
            if (builtins_map.count(command[0])) {
                builtins_map[command[0]](command);
            }
            else {
                pid_child = fork();
                if (pid_child < 0) {
                    std::cerr << "[!] Error creating fork" << "\n";
                } else if (pid_child == 0) {
                    if (execvp(command[0], command.data()) < 0) {
                        std::cerr << "[!] Error executing the command " << command[0] << "\n";
                    }
                } else {
                    wait_result = waitpid(pid_child, &stat_loc, WUNTRACED);
                }
            }
        }
    }    
    return 0;
}
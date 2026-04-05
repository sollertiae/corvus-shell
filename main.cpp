#include <iostream>
#include <unistd.h> //required for forks
#include <sys/wait.h> //required for wait
#include <string> 
#include <vector>
#include <map>
#include <functional>

int builtin_cd(std::vector<char*>& args) {
    if (args.size() < 2) {
        std::cerr << "cd: missing argument" << "\n";
        return -1;        
    }
    return chdir(args[1]);
}
int builtin_exit(std::vector<char*>& args);

std::map<std::string, std::function<int(std::vector<char*>&) >> builtins_map = {
    {"cd", builtin_cd}
};

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
        command = read_input(input.data());
        
        if (builtins_map.count(command[0])) {
            builtins_map[command[0]](command);
        }
        else {
            pid_child = fork();
            if (pid_child < 0) {
                std::cerr << "[!] Error creating fork" << "\n";
            } else if (pid_child == 0) {
                std::cout << "[+] Executing the command on child fork" << "\n";
                if (execvp(command[0], command.data()) < 0) {
                    std::cerr << "[!] Error executing the command" << command[0] << "\n";
                }
            } else {
                wait_result = waitpid(pid_child, &stat_loc, WUNTRACED);
            }
        }
    }    
    return 0;
}
#include "builtins.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

std::map<std::string, std::function<int(std::vector<char*>&) >> builtins_map = {
    {"cd", builtin_cd},
    {"exit", builtin_exit},
    {"pwd", builtin_pwd},
    {"help", builtin_help}
};

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

int builtin_pwd(std::vector<char*>& args) {
    char cwd[512];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        std::cout << cwd << "\n";
        return 0;
    }
    std::cerr << "corvus: pwd: error" << "\n";
    return -1;
}

//TODO: Enhance map to store a brief description of each command
int builtin_help(std::vector<char*>& args) {
    std::cout << "Corvus shell" << "\n";
    std::cout << "Built-in commands:" << "\n";
    for (const auto& [name, fn] : builtins_map) {
        std::cout << " " << name << "\n";
    }
    return 0;
}
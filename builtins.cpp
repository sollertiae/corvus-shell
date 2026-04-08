#include "builtins.h"
#include <iostream>
#include <unistd.h>
#include <cstdlib>

std::map<std::string, std::function<int(std::vector<char*>&) >> builtins_map = {
    {"cd", builtin_cd},
    {"exit", builtin_exit}
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
#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

struct Builtin {
    std::function<int(std::vector<char*>&)> fn;
    std::string description;
};

int builtin_cd(std::vector<char*>& args);
int builtin_exit(std::vector<char*>& args);
int builtin_pwd(std::vector<char*>& args);
int builtin_help(std::vector<char*>& args);

extern std::map<std::string, Builtin> builtins_map;

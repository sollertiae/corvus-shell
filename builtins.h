#pragma once
#include <vector>
#include <string>
#include <map>
#include <functional>

int builtin_cd(std::vector<char*>& args);
int builtin_exit(std::vector<char*>& args);

extern std::map<std::string, std::function<int(std::vector<char*>&)>> builtins_map;
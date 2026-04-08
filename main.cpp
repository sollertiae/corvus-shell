#include <iostream>
#include <unistd.h> //required for forks
#include <sys/wait.h> //required for wait
#include <string> 
#include <vector>
#include <map>
#include <functional>
#include <cstdlib>
#include "parser.h"
#include "builtins.h"
#include "executor.h"

int main() {
    std::string input;
    while (true) {
        std::cout << "corvus >> ";
        std::getline(std::cin, input);

        if (input.empty())
            continue;

        ParsedPipeline parsed_input = parse_pipeline(input);
        if (parsed_input.commands.size() == 1) {
            if(builtins_map.count(parsed_input.commands[0][0])) {
                builtins_map[parsed_input.commands[0][0]](parsed_input.commands[0]);
            } else {
                execute_command(parsed_input.commands[0]);
                waitpid(-1, nullptr, 0);
            }
        } else {
            execute_pipeline(parsed_input.commands);
        }
    }    
    return 0;
}
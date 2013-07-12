#include <sstream>
#include <algorithm>

#include "parser.h"
#include "help_functions.h"

namespace spel {
    
Parser::Parser(Main_Actor * v) : main_character(v) { }

string Parser::read() {
    while (true) {
        cout << ">>";
        string command;
        getline(cin, command);
        if (command.size() > 0)
            return lowercase(command);   
        }
    }
}

} // End of namespace

using namespace spel;

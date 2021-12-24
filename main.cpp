#include <iostream>
#include <string>
#include "NetConfAgent.hpp"
#include "MobileClient.hpp"

void twoWordCommands(mobilenetwork::MobileClient &mc, std::string const &command, std::string &value) {
    if (command == "register")
        mc.Register(value);
    else if (command == "setName")
        mc.setName(value);
    else if (command == "call")
        mc.call(value);
    else
        std::cout << "wrong command!" << std::endl;
}

int main() {
    std::string line;
    std::cout << "options: register 'number', unregister, setName 'name'," 
     << " call 'number', callEnd, answer, regect, exit" << std::endl;
    mobilenetwork::MobileClient mc;
    bool cycle = true;
    while (cycle) {
        getline(std::cin, line);
        size_t pos;
        if ((pos = line.find(' ')) != std::string::npos) {
            std::string word1 = line.substr(0, pos);
            std::string word2 = line.substr(pos + 1);
            size_t pos2;
            if ((pos2 = word2.find(' ')) != std::string::npos)
            {
                std::cout << "wrong format: 'command' + 'arg' / 'commmand'" << std::endl;
                continue;
            }
            twoWordCommands(mc, word1, word2);
        }
        else {
            if (line == "register" || line == "setName" || line == "call")
                std::cout << "wrong argument: this command need an argument" << std::endl;
            else if (line == "unregister")
                mc.unregister();
            else if (line == "callEnd")
                mc.callEnd();
            else if (line == "answer")
                mc.answer();
            else if (line == "regect")
                mc.regect();
            else if (line == "exit")
            {
                mc.safeExit();
                cycle = false;
            }
            else
                std::cout << "wrong command!" << std::endl;
        }

    }
    return 0;
}
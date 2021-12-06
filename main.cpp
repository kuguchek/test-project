#include <iostream>
#include <string>
#include <sstream>
#include "NetConfAgent.hpp"
#include "MobileClient.hpp"

bool unregister() {
    std::cout << "unregistred!" << std::endl;
    return true;
}

void callEnd() {
    std::cout << "end call?" << std::endl;
}

void regect() {
    std::cout << "regect?" << std::endl;
}

void twoWordCommands(MobileClient & mc, std::string const & command, std::string & value) {
    if (command == "register") {
        if (mc.Register(value))
            std::cout << "> number " << value << " registred succesfully!" << std::endl;
    }
    else if (command == "setName")
        mc.setName(value);
    else if (command == "call") {
        if(!mc.call(value))
            std::cout << "> subscriber is unavailable for some reasons! can't call! " << value << std::endl;
    }
    else
        std::cout << "wrong command!" << std::endl;
}

int main() {
    std::string line;
    std::cout << "options: register 'number', unregister, setName 'name'," 
     << " call 'number', callEnd, answer, regect, exit" << std::endl;
    MobileClient mc;
    while (1) {
        getline(std::cin, line);
        size_t pos;
        if ((pos = line.find(' ')) != std::string::npos) {
            std::string word1 = line.substr(0, pos);
            std::string word2 = line.substr(pos + 1);
            size_t pos2;
            if ((pos2 = word2.find(' ')) != std::string::npos) //if more than 2 words - error 
            {
                std::cout << "wrong format: 'command' 'arg' / 'commmand'" << std::endl;
                continue;
            }
            twoWordCommands(mc, word1, word2);
        }
        else {
            if (line == "register" || line == "setName" || line == "call")
                std::cout << "wrong argument: 'command' 'arg'" << std::endl;
            else if (line == "unregister")
                unregister();
            else if (line == "callEnd")
                callEnd();
            else if (line == "answer")
                mc.answer();
            else if (line == "regect")
                regect();
            else if (line == "exit")
                break;
            else
                std::cout << "wrong command!" << std::endl;
        }

    }
    return 0;
}
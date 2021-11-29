#include <iostream>
#include <string>
#include <sstream>
#include "NetConfAgent.hpp"

bool Register(std::string number) {
    std::cout << number << " registred succesfully!" << std::endl;
    return true;
}

bool unregister() {
    std::cout << "unregistred!" << std::endl;
    return true;
}

void setName(std::string name) {
    std::cout << "name: " << name << " added!" << std::endl;
}

void call(std::string incomingNumber) {
    std::cout << "calling " << incomingNumber << std::endl;
}

void callEnd() {
    std::cout << "end call?" << std::endl;
}

void answer() {
    std::cout << "answer?" << std::endl;
}

void regect() {
    std::cout << "regect?" << std::endl;
}

int main() {
    NetConfAgent nca;
    //nca.subscribeForModelChanges();
    //std::string str, str2;
    //str = "/mobilenetwork:subscribers/subscriber[number='001']/number"; //fetchData() tests
    //nca.fetchData(str, str2);
    //str = "/mobilenetwork:subscribers/subscriber[number='001']/incomingNumber";
    //str2 = "000";
    //nca.changeData(str, str2);
    nca.subscribeOnOper();
    std::string line;
    std::cout << "options: register 'number', unregister, setName 'name'," 
     << " call 'number', callEnd, answer, regect, exit" << std::endl;
    while (1) {
        getline(std::cin, line);
        size_t pos;
        if ((pos = line.find(' ')) != std::string::npos) { //while more than one word - "register", "setName", "call" 
            std::string word1 = line.substr(0, pos);
            std::string word2 = line.substr(pos + 1);
            if ((pos = word2.find(' ')) != std::string::npos) //if more than 2 words - error
                std::cout << "wrong format: 'command' 'arg' / 'commmand'" << std::endl;
            if (word1 == "register")
                Register(word2);
            else if (word1 == "setName")
                setName(word2);
            else if (word1 == "call")
                call(word2);
            else
                std::cout << "wrong command!" << std::endl;
        }
        else {
            if (line == "register" || line == "setName" || line == "call")
                std::cout << "wrong argument: 'command' 'arg'" << std::endl;
            else if (line == "unregister")
                unregister();
            else if (line == "callEnd")
                callEnd();
            else if (line == "answer")
                answer();
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
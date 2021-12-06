#include "MobileClient.hpp"
#include "NetConfAgent.hpp"
#include "Constants.hpp"

MobileClient::MobileClient() : _state(State::idle) {
    _netConf = std::make_unique<NetConfAgent>();
}

void MobileClient::setName(std::string & name) {
    _name = name;
}

bool MobileClient::Register(std::string & number) {
    std::string str = "";
    std::string path = makePath(number, Leaf::number);

    if (_netConf->fetchData(makePath(number, Leaf::number), str) == false) //no data on this path
    {
        _number = number;
        _netConf->subscribeForModelChanges(makePath(number, Leaf::subscriber), *this);
        _netConf->changeData(path, _number);
        return true;
    }
    else {
        std::cout << number << " already registred!" << std::endl;
        return false;
    }
}

bool MobileClient::call(std::string incomingNumber) {
    if (_number.empty() || _state != State::idle)
        return false;
    std::string str = "";
    if (_netConf->fetchData(makePath(incomingNumber, Leaf::number), str) == false) //no such number
        return false;
    else {
        std::string state = "";
        std::string statePath = makePath(incomingNumber, Leaf::state);
        _netConf->fetchData(statePath, state);
        if (state == "idle")
        {
            _state = State::active;
            _netConf->changeData(statePath, "active");
            _netConf->changeData(makePath(_number, Leaf::state), "active"); //change my state
            _netConf->changeData(makePath(incomingNumber, Leaf::incomingNumber), _number);
        }
        else
            return false; //std::cout << "number is busy/active" << std::endl;
    }
    return true;
}

void MobileClient::answer() {
    std::string value = "";

    if (_netConf->fetchData(makePath(_number, Leaf::incomingNumber), value) == false) // answer only when you have incoming call
        std::cout << "there is no incoming call!" << std::endl;
    else {
        _incomingNumber = value;
        _state = State::busy;
        _netConf->changeData(makePath(_number, Leaf::state), "busy");
        _netConf->changeData(makePath(_incomingNumber, Leaf::state), "busy");
    }
}

std::string MobileClient::makePath(std::string & value, Leaf type) {
    std::string end;

    switch(type) {
        case Leaf::number :
            end = "']/number";
            break;
        case Leaf::userName : 
            end = "']/userName";
            break;
        case Leaf::incomingNumber :
            end = "']/incomingNumber";
            break;
        case Leaf::state :
            end = "']/state";
            break;
        case Leaf::subscriber :
            end = "']";
            break;
    }
    return (pattern + value + end);
}

void MobileClient::handleModuleChange(std::string & path, std::string & value) {
    std::size_t pos;
    if ((pos = path.find("incomingNumber")) != std::string::npos) {
        std::cout << "> incoming call from " << value <<std::endl;
    }
    if ((pos = path.find("state") != std::string::npos) && value == "busy") {
        std::cout << "> connected! *some conversation here*" << std::endl;
    }
    std::cout << "path: " << path << std::endl;
    std::cout << "value: " << value << std::endl;
}
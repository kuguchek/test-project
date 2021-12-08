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
    std::string str;
    std::string path = makePath(number, Leaf::number);

    if (_netConf->fetchData(makePath(number, Leaf::number), str) == false) //no data on this path
    {
        _number = number;
        _netConf->changeData(path, _number);
        _netConf->subscribeForModelChanges(makePath(number, Leaf::subscriber), *this);
        std::cout << redBold << "> " << number << " registred succesfully!" << resetCol << std::endl;
        return true;
    }
    else {
        std::cout << redBold << "> " << number << " already registred!" << resetCol <<std::endl;
        return false;
    }
}

bool MobileClient::call(std::string incomingNumber) {
    if (_number.empty() || _state != State::idle) {
        std::cout << redBold << "> unavailable for call" << resetCol <<std::endl;
        return false;
    }
    std::string str;
    if (_netConf->fetchData(makePath(incomingNumber, Leaf::number), str) == false) { //no such number
        std::cout << redBold << "> no such number for call:"  << str << resetCol <<std::endl;
        return false;
    }
    else {
        std::string state;
        std::string statePath = makePath(incomingNumber, Leaf::state);
        _netConf->fetchData(statePath, state);
        if (state == "idle")
        {
            _outgoingNumber = incomingNumber;
            _netConf->changeData(statePath, enumStr[State::active]);
            _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::active]); //change my state
            _netConf->changeData(makePath(incomingNumber, Leaf::incomingNumber), _number);
        }
        else {
            std::cout << redBold << "> number is busy/active: "  << str << resetCol <<std::endl;
            return false;
        }
    }
    return true;
}

void MobileClient::answer() {
    std::string value;
    if (_incomingNumber.empty()) // answer only when you have incoming call
        std::cout << redBold << "> there is no incoming call!" << resetCol << std::endl;
    else {
        _netConf->changeData(makePath(_number, Leaf::state), "busy");
        _netConf->changeData(makePath(_incomingNumber, Leaf::state), "busy");
    }
}

void MobileClient::regect() {
    std::string value;
    if (_incomingNumber.empty()) // regect only when you have incoming call
        std::cout << redBold << "> there is no incoming call!" << resetCol << std::endl;
    else {
        _netConf->changeData(makePath(_number, Leaf::incomingNumber), "");
        //_netConf->deleteData(makePath(_number, Leaf::incomingNumber));
        _netConf->changeData(makePath(_incomingNumber, Leaf::state), enumStr[State::idle]);
        _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::idle]);
    }
}

void MobileClient::callEnd() { //if conversation
    if (_state == State::busy) {
        if (_incomingNumber.empty() == false) {//if it's incoming call
            _netConf->changeData(makePath(_number, Leaf::incomingNumber), "");
            _netConf->changeData(makePath(_incomingNumber, Leaf::state), enumStr[State::idle]);
        }
        else if (_outgoingNumber.empty() == false) { //if we calling
            _netConf->changeData(makePath(_outgoingNumber, Leaf::incomingNumber), "");
            _netConf->changeData(makePath(_outgoingNumber, Leaf::state), enumStr[State::idle]);
        }
        _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::idle]);
    }
    else
        std::cout << redBold << "> there is no call to end!" << resetCol << std::endl;
}

void MobileClient::unregister() {
    if (_number.empty() == false) { //unregister only for registred
        _netConf->deleteData(makePath(_number, Leaf::subscriber));
        _netConf->closeSubscription();
        _number.clear();
        std::cout << redBold << "> unregistered!" << resetCol << std::endl;
    }
    else
        std::cout << redBold << "> can't unregister!" << resetCol << std::endl;
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
    return (_pattern + value + end);
}

void MobileClient::handleModuleChange(std::string & path, std::string & value) {
    std::size_t pos, pos2;
    if ((pos = path.find("incomingNumber") != std::string::npos) && (value.empty() == false)) {
        _incomingNumber = value;
        std::cout << redBold << "> incoming call from " << value << resetCol <<std::endl;
    }
    else if ((pos2 = path.find("state") != std::string::npos)) {
        if (value == enumStr[State::busy]) {
            _state = State::busy;
            std::cout << redBold << "> connected! *some conversation here*" << resetCol << std::endl;
        }
        else if (value == enumStr[State::active]) {
            _state = State::active;
        }
        else { //idle
            _state = State::idle;
            if (_incomingNumber.empty() == false || _outgoingNumber.empty() == false) {
                if (_incomingNumber.empty() == false) 
                 _incomingNumber.clear();
                else //_outgoingNumber.empty() == false)
                    _outgoingNumber.clear();
                std::cout << redBold << "> end of the call" << resetCol << std::endl;
            }
        }
    }
    //std::cout << "path: " << path << std::endl;
    //std::cout << "value: " << value << std::endl;
}
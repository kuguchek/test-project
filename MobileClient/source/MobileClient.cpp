#include <sysrepo-cpp/utils/exception.hpp>
#include <map>
#include "MobileClient.hpp"
#include "NetConfAgent.hpp"
#include "Constants.hpp"

namespace mobilenetwork {
MobileClient::MobileClient() : MobileClient(std::make_unique<NetConfAgent>()) {
}

MobileClient::MobileClient(std::unique_ptr<INetConfAgent> ptr) : _netConf{std::move(ptr)} {
    _state = State::idle;
    _startTime = 0;
    _endTime = 0;
}

void MobileClient::setName(const std::string &name) {
    _name = name;
    std::cout << redBold << "> " << _name << " - name aded!" << resetCol << std::endl;
}

std::string MobileClient::getName() {
    return _name;
}

bool MobileClient::Register(const std::string &number) {
    std::string str;
    std::string path = makePath(number, Leaf::number);

    if (_name.empty()) {
        std::cout << redBold << "set name ('setName' command) before registration!" << resetCol << std::endl;
        return false;
    }
    if (_netConf->fetchData(makePath(number, Leaf::number), str) == false)
    {
        try {
            _netConf->changeData(path, _number);
        }
        catch (sysrepo::ErrorWithCode & e) {
            std::cout << redBold << number << " can't be setted" << resetCol <<std::endl;
            return false;
        }
        _number = number;
        _netConf->subscribeForModelChanges(makePath(number, Leaf::subscriber), *this);
        if (_name.empty() == false)
            _netConf->registerOperData(makePath(number, Leaf::userName), *this);
        std::cout << redBold << "> " << number << " registred succesfully!" << resetCol << std::endl;
        return true;
    }
    else {
        std::cout << redBold << "> " << number << " already registred!" << resetCol <<std::endl;
        return false;
    }
}

bool MobileClient::call(const std::string incomingNumber) {
    if (_number.empty() || _state != State::idle || _number == incomingNumber) {
        std::cout << redBold << "> unavailable for call" << resetCol <<std::endl;
        return false;
    }
    std::string str;
    if (_netConf->fetchData(makePath(incomingNumber, Leaf::number), str) == false) {
        std::cout << redBold << "> no such number for call: " << incomingNumber << resetCol <<std::endl;
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
            _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::active]);
            _netConf->changeData(makePath(incomingNumber, Leaf::incomingNumber), _number);
        }
        else {
            std::cout << redBold << "> number is busy/active: "  << str << resetCol <<std::endl;
            return false;
        }
    }
    return true;
}

bool MobileClient::answer() {
    std::string strTime;
    if (_state != State::active || (_incomingNumber.empty() == true)) {
        std::cout << redBold << "> there is no incoming call!" << resetCol << std::endl;
        return false;
    }
    else {
        _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::busy]);
        _netConf->changeData(makePath(_incomingNumber, Leaf::state), enumStr[State::busy]);
        strTime = ctime(&_startTime);
        std::map<std::string, std::string> map;
        map["startTime"] = strTime;
        map["abonentA"] = _number;
        map["abonentB"] = _incomingNumber;
        _netConf->notifySysrepo(map);
    }
    return true;
}

bool MobileClient::regect() {
    if (_state != State::active || (_incomingNumber.empty() == true)) {
        std::cout << redBold << "> there is no incoming call!" << resetCol << std::endl;
        return false;
    }
    else {
        _netConf->changeData(makePath(_number, Leaf::incomingNumber), "");
        _netConf->changeData(makePath(_incomingNumber, Leaf::state), enumStr[State::idle]);
        _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::idle]);
    }
    return true;
}

bool MobileClient::callEnd() {
    std::map<std::string, std::string> map;

    if (_state == State::busy) {
        std::string strTime = ctime(&_startTime);
        map["startTime"] = strTime;
        map["abonentA"] = _number;
        if (_incomingNumber.empty() == false) {
            map["abonentB"] = _incomingNumber;
            _netConf->changeData(makePath(_number, Leaf::incomingNumber), "");
            _netConf->changeData(makePath(_incomingNumber, Leaf::state), enumStr[State::idle]);
        }
        else if (_outgoingNumber.empty() == false) {
            map["abonentB"] = _outgoingNumber;
            _netConf->changeData(makePath(_outgoingNumber, Leaf::incomingNumber), "");
            _netConf->changeData(makePath(_outgoingNumber, Leaf::state), enumStr[State::idle]);
        }
        _netConf->changeData(makePath(_number, Leaf::state), enumStr[State::idle]);
        map["duration"] = std::to_string(difftime(_endTime, _startTime));
        _netConf->notifySysrepo(map);
    }
    else {
        std::cout << redBold << "> there is no call to end!" << resetCol << std::endl;
        return false;
    }
    return true;
}

bool MobileClient::unregister() {
    if (_number.empty() == false && _state == State::idle) {
        _netConf->deleteData(makePath(_number, Leaf::subscriber));
        _netConf->closeSubscription();
        _number.clear();
        _name.clear();
        std::cout << redBold << "> unregistered!" << resetCol << std::endl;
    }
    else {
        std::cout << redBold << "> can't unregister!" << resetCol << std::endl;
        return false;
    }
    return true;
}

std::string MobileClient::makePath(const std::string &value, Leaf type) {
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

void MobileClient::handleModuleChange(const std::string &path, const std::string &value) {
    std::size_t pos, pos2;
    if ((pos = path.find("incomingNumber") != std::string::npos) && (value.empty() == false)) {
        _incomingNumber = value;
        std::string name;
        _netConf->fetchData(makePath(_incomingNumber, Leaf::userName), name);
        std::cout << redBold << "> incoming call from " << value << ", name: " << name << resetCol <<std::endl;
    }
    else if ((pos2 = path.find("state") != std::string::npos)) {
        if (value == enumStr[State::busy]) {
            _state = State::busy;
            time(&_startTime);
            std::cout << redBold << "> connected! *some conversation here*" << resetCol << std::endl;
        }
        else if (value == enumStr[State::active]) {
            _state = State::active;
        }
        else {
            _state = State::idle;
            time(&_endTime);
            if (_incomingNumber.empty() == false || _outgoingNumber.empty() == false) {
                if (_incomingNumber.empty() == false) 
                    _incomingNumber.clear();
                else
                    _outgoingNumber.clear();
                std::cout << redBold << "> end of the call" << resetCol << std::endl;
            }
        }
    }
}
void MobileClient::safeExit() {
    if (_incomingNumber.empty() == false)
        _netConf->changeData(makePath(_incomingNumber, Leaf::state), enumStr[State::idle]);
    else if (_outgoingNumber.empty() == false)
    {
        _netConf->changeData(makePath(_outgoingNumber, Leaf::state), enumStr[State::idle]);
        _netConf->changeData(makePath(_outgoingNumber, Leaf::incomingNumber), "");
    }
    if (_number.empty() == false)
    {
        _netConf->deleteData(makePath(_number, Leaf::subscriber));
        _netConf->closeSubscription();
    }
}
MobileClient::~MobileClient() = default;
}
#include "MobileClient.hpp"

MobileClient::MobileClient() : _state(State::idle) {
    _netConf = std::make_unique<NetConfAgent>();
}

void MobileClient::setName(std::string & name) {
    _name = name;
}

bool MobileClient::Register(std::string & number) {
    std::string str = "";
    std::string path = makePath(number, Leaf::number);

    if (_netConf->fetchData(path, str) == false) //no data on this path
    {
        _number = number;
        std::string path2 = makePath(number, Leaf::subscriber);
        _netConf->subscribeForModelChanges(path2);
        _netConf->changeData(path, _number);
        return true;
    }
    else {
        std::cout << number << " already registred!" << std::endl;
        return false;
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
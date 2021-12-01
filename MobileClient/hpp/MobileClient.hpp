#include "NetConfAgent.hpp"

enum class State {
    idle,
    busy,
    active
};

enum class Leaf {
    number,
    userName,
    incomingNumber,
    state,
    subscriber
};

class MobileClient {
    public:
        MobileClient();
        void setName(std::string & name);
        bool Register(std::string & number);
    private:
        std::string makePath(std::string & value, Leaf type);
        std::string _name;
        std::string _number;
        std::string _incomingNumber;
        State _state;
        std::unique_ptr<NetConfAgent> _netConf;
        const std::string pattern = "/mobilenetwork:subscribers/subscriber[number='";
};
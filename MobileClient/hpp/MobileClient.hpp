# ifndef MOBILECLIENT_HPP
# define MOBILECLIENT_HPP

#include <memory>
#include <string>

class NetConfAgent;

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
        void handleModuleChange(std::string & path, std::string & value);
        bool call(std::string incomingNumber);
        void answer();
        void regect();
        void callEnd();
        void unregister();
    private:
        std::string makePath(std::string & value, Leaf type);
        std::string _name;
        std::string _number;
        std::string _incomingNumber;
        std::string _outgoingNumber;
        State _state;
        std::unique_ptr<NetConfAgent> _netConf;
};

#endif
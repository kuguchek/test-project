# ifndef NETCONFAGENT_HPP
# define NETCONFAGENT_HPP
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <iostream>
#include <optional>

class NetConfAgent {
    public:
        NetConfAgent();
        void subscribeForModelChanges();
        bool fetchData(std::string & path, std::string & str);
        void changeData(std::string & path, std::string & value);
        void subscribeOnOper();
    private:
        sysrepo::Connection _con;
        sysrepo::Session _ses;
        std::optional<sysrepo::Subscription> _sub;
};

#endif
# ifndef NETCONFAGENT_HPP
# define NETCONFAGENT_HPP
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <iostream>
#include <optional>

class MobileClient;

class NetConfAgent {
    public:
        NetConfAgent();
        void subscribeForModelChanges(std::string const & path, MobileClient & mc);
        bool fetchData(std::string const & path, std::string & str);
        void changeData(std::string const & path, std::string const & value);
        void registerOperData();
    private:
        sysrepo::Connection _con;
        sysrepo::Session _ses;
        std::optional<sysrepo::Subscription> _sub;
};

#endif
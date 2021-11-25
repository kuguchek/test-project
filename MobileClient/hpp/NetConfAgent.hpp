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
    private:
        sysrepo::Connection con;
        sysrepo::Session ses;
        std::optional<sysrepo::Subscription> sub;
};

#endif
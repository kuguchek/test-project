# ifndef NETCONFAGENT_HPP
# define NETCONFAGENT_HPP
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <iostream>

class NetConfAgent {
    public:
        void init();
        void subscribeForModelChanges();
    private:
        std::unique_ptr<sysrepo::Connection> con;
};

#endif
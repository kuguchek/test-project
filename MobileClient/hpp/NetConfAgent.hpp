#pragma once
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <iostream>

class NetConfAgent {
    public:
        bool subscribeForModelChanges();
    private:
        sysrepo::Connection con;
        sysrepo::Session sess;
        sysrepo::Subscription sub;

};
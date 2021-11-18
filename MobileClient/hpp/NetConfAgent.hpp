#pragma once
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>

class NetConfAgent {

    private:
        sysrepo::Connection con;
        sysrepo::Session sess;
        sysrepo::Subscription sub;

};
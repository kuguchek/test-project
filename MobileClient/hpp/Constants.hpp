# ifndef CONSTANTS_HPP
# define CONSTANTS_HPP

#include <iostream>
#include <map>

namespace mobilenetwork {
    enum class State;
}

namespace {
    std::map<mobilenetwork::State, std::string> enumStr {
        {mobilenetwork::State::active, "active"},
        {mobilenetwork::State::busy, "busy"},
        {mobilenetwork::State::idle, "idle"}
    };
    const std::string _pattern = "/commutator:subscribers/subscriber[number='";
    const std::string _moduleName = "commutator";
    const std::string redBold = "\033[1;31m";
    const std::string resetCol = "\033[0m";
    const std::string notificationPath = "/commutator:statCall";
}

#endif
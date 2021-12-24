# ifndef NETCONFAGENTMOCK_HPP
# define NETCONFAGENTMOCK_HPP

#include <string>
#include <gmock/gmock.h>
#include "INetConfAgent.hpp"

namespace mock {
class NetConfAgentMock :  public mobilenetwork::INetConfAgent {
    public:
        MOCK_METHOD2(subscribeForModelChanges, void(std::string const &path, mobilenetwork::MobileClient &mc));
        MOCK_METHOD2(fetchData, bool(std::string const &path, std::string &str));
        MOCK_METHOD2(changeData, void(std::string const &path, std::string const &value));
        MOCK_METHOD1(deleteData, void(std::string path));
        MOCK_METHOD2(registerOperData, void(const std::string &path, mobilenetwork::MobileClient &mc));
        MOCK_METHOD0(closeSubscription, void());
        MOCK_METHOD1(notifySysrepo, void(std::map<std::string, std::string> map));
};
}
#endif
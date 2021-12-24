#include <map>
#include "NetConfAgent.hpp"
#include "MobileClient.hpp"
#include "Constants.hpp"

namespace mobilenetwork{
NetConfAgent::NetConfAgent() : _con(), _ses(_con.sessionStart()) { }

void NetConfAgent::closeSubscription() {
    _sub = std::nullopt;
    _subOper = std::nullopt;
}

void NetConfAgent::registerOperData(const std::string &path, MobileClient &mc) {
        sysrepo::OperGetItemsCb operGetCb = [&, path] (sysrepo::Session ses, auto, auto, auto, auto, auto, std::optional<libyang::DataNode>& parent) {
            std::string value = mc.getName();
            parent = ses.getContext().newPath(path.c_str(), value.c_str());
            return sysrepo::ErrorCode::Ok;;
    };
    _subOper = _ses.onOperGetItems(_moduleName.c_str(), operGetCb, path.c_str());
}

void NetConfAgent::deleteData(const std::string path) {
    _ses.deleteItem(path.c_str());
    _ses.applyChanges();
}

void NetConfAgent::changeData(const std::string &path, const std::string &value) {
    _ses.setItem(path.c_str(), value.c_str());
    _ses.applyChanges();
}

bool NetConfAgent::fetchData(const std::string &path, std::string &str) {
    auto data = _ses.getData(path.c_str());
    if (!data.has_value()) {
        _ses.switchDatastore(sysrepo::Datastore::Operational);
        auto data2 = _ses.getData(path.c_str());
        if (!data2.has_value()) {
            _ses.switchDatastore(sysrepo::Datastore::Running);
            return false;
        }
        str = data2->findPath(path.c_str()).value().asTerm().valueStr();
        _ses.switchDatastore(sysrepo::Datastore::Running);
        return true;
    }
    str = data->findPath(path.c_str()).value().asTerm().valueStr();
    return true;
}

void NetConfAgent::notifySysrepo(std::map<std::string, std::string> map) {
    auto notification = _ses.getContext().newPath(notificationPath.c_str());
    for (auto & m : map) {
        notification.newPath((m.first).c_str(), (m.second).c_str());
    }
    _ses.sendNotification(notification, sysrepo::Wait::Yes);
}

void NetConfAgent::subscribeForModelChanges(const std::string &path, MobileClient &mc) {
    sysrepo::ModuleChangeCb changeCb = [&mc] (sysrepo::Session ses, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
            for (auto changes : ses.getChanges())
            {
                std::string changePath = std::string{changes.node.path()};
                std::string value = "";
                if (changes.node.schema().nodeType() == libyang::NodeType::Leaf &&
                            changes.operation != sysrepo::ChangeOperation::Deleted) {
                    value = std::string{changes.node.asTerm().valueStr()};
                    mc.handleModuleChange(changePath, value);
                }
            }
            return sysrepo::ErrorCode::Ok;
    };
    _sub = _ses.onModuleChange(_moduleName.c_str(), changeCb, path.c_str(), 0, sysrepo::SubscribeOptions::DoneOnly);
}

NetConfAgent::~NetConfAgent() = default;
}
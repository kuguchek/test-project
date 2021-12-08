#include "NetConfAgent.hpp"
#include "MobileClient.hpp"
#include "Constants.hpp"

NetConfAgent::NetConfAgent() : _con(), _ses(_con.sessionStart()) { }

void NetConfAgent::closeSubscription() {
    _sub = std::nullopt;
}

void NetConfAgent::registerOperData() {
        sysrepo::ErrorCode ret;
        sysrepo::OperGetItemsCb operGetCb = [&] (sysrepo::Session _ses, auto, auto, auto, auto, auto, std::optional<libyang::DataNode>& parent) {
            parent = _ses.getContext().newPath("/mobilenetwork:subscribers/subscriber[number='124']/userName", "name14");
            std::cout << "has value: " << parent.has_value() << std::endl;
            ret = sysrepo::ErrorCode::Ok;
            return ret;
    };
    _sub = _ses.onOperGetItems(_moduleName.c_str(), operGetCb, "/mobilenetwork:*");
    //_ses.switchDatastore(sysrepo::Datastore::Operational);
}

void NetConfAgent::deleteData(std::string path) {
    _ses.deleteItem(path.c_str());
    _ses.applyChanges();
}

void NetConfAgent::changeData(std::string const & path, std::string const & value) {
    //const char *cpath = path.c_str();
    //const char *cval = value.c_str();
    _ses.setItem(path.c_str(), value.c_str());
    _ses.applyChanges();
}

bool NetConfAgent::fetchData(std::string const & path, std::string & str) {
    const char *cstr = path.c_str();
    auto data = _ses.getData(cstr);
    if (!data) // == nullopt?;
        return false;
    str = data->findPath(cstr).value().asTerm().valueStr();
    return true;
}

void NetConfAgent::subscribeForModelChanges(std::string const & path, MobileClient & mc) {
    _ses.copyConfig(sysrepo::Datastore::Running, _moduleName.c_str());
    sysrepo::ModuleChangeCb changeCb = [&mc] (sysrepo::Session _ses, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
            for (auto changes : _ses.getChanges())
            {
                if (changes.operation == sysrepo::ChangeOperation::Deleted)
                    std::cout << "subscribeForModelChanges() -> delete" << std::endl;
                std::string changePath = std::string{changes.node.path()};
                std::cout << "on path: " << changePath << std::endl;
                std::string value = "";
                if (changes.node.schema().nodeType() == libyang::NodeType::Leaf) {
                    value = std::string{changes.node.asTerm().valueStr()};
                    mc.handleModuleChange(changePath, value);
                }
            }
            return sysrepo::ErrorCode::Ok;
    };
    _sub = _ses.onModuleChange(_moduleName.c_str(), changeCb, path.c_str(), 0, sysrepo::SubscribeOptions::DoneOnly);
}

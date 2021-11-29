#include "NetConfAgent.hpp"

NetConfAgent::NetConfAgent() : _con(), _ses(_con.sessionStart()) { }

void NetConfAgent::subscribeOnOper() {
        sysrepo::ErrorCode ret;
        sysrepo::OperGetItemsCb operGetCb = [&] (sysrepo::Session _ses, auto, auto, auto, auto, auto, std::optional<libyang::DataNode>& parent) {
            parent = _ses.getContext().newPath("/mobilenetwork:subscribers/subscriber[number='008']/userName", "name8");
            std::cout << "has value: " << parent.has_value() << std::endl;
            ret = sysrepo::ErrorCode::Ok;
            return ret;
    };
    _sub = _ses.onOperGetItems("mobilenetwork", operGetCb, "/mobilenetwork:*");
    //_ses.switchDatastore(sysrepo::Datastore::Operational);
}

void NetConfAgent::changeData(std::string & path, std::string & value) {
    const char *cpath = path.c_str();
    const char *cval = value.c_str();
    _ses.setItem(cpath, cval);
    _ses.applyChanges();
}

bool NetConfAgent::fetchData(std::string & path, std::string & str) {
    const char *cstr = path.c_str();
    auto data = _ses.getData(cstr);
    //data.has_value();
    if (!data) // == nullopt?;
        return false;
    //str = std::string{data->path()}; -> /mobilenetwork:subscribers
    str = data->findPath(cstr).value().asTerm().valueStr();
    std::cout << str << std::endl;
    return true;
}

void NetConfAgent::subscribeForModelChanges() { //first @par string path, bool? map[]?
    _ses.copyConfig(sysrepo::Datastore::Running, "mobilenetwork");
    sysrepo::ModuleChangeCb changeCb = [] (sysrepo::Session _ses, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
            for(auto changes : _ses.getChanges("/mobilenetwork:subscribers/subscriber[number='002']/incomingNumber"))
            {
                std::cout << "created: " << (changes.operation == sysrepo::ChangeOperation::Created) << std::endl;
                std::cout << "modified: " << (changes.operation == sysrepo::ChangeOperation::Modified) << std::endl;
                std::cout << std::string{changes.node.path()} << std::endl;
            }
            return sysrepo::ErrorCode::Ok;
    };
    _sub = _ses.onModuleChange("mobilenetwork", changeCb, "/mobilenetwork:subscribers/subscriber[number='002']/incomingNumber");
}

#include "NetConfAgent.hpp"

NetConfAgent::NetConfAgent() : con(), ses(con.sessionStart()) {

}

bool NetConfAgent::fetchData(std::string & path, std::string & str) {
    const char *cstr = path.c_str();
    auto data = ses.getData(cstr);
    if (!data)
        return false;
    //str = std::string{data->path()}; -> /mobilenetwork:subscribers
    str = data->findPath(cstr).value().asTerm().valueStr();
    std::cout << str << std::endl;
    return true;
}

void NetConfAgent::subscribeForModelChanges() { //bool?
    ses.copyConfig(sysrepo::Datastore::Running, "mobilenetwork");
    sysrepo::ModuleChangeCb changeCb = [] (sysrepo::Session, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {

            return sysrepo::ErrorCode::Ok;
    };
    sub = ses.onModuleChange("mobilenetwork", changeCb);
}

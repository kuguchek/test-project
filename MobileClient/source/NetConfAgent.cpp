#include "../hpp/NetConfAgent.hpp"

void NetConfAgent::init() {
    con = std::make_unique<sysrepo::Connection>();
}

void NetConfAgent::subscribeForModelChanges() {
    init();
    auto sess = con->sessionStart();
    sess.copyConfig(sysrepo::Datastore::Startup, "mobilenetwork");
    int32_t called = 0;

    sysrepo::ModuleChangeCb changeCb = [&called] (auto, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
            called++;
            //std::cout << "called: " << called << std::endl;
            return sysrepo::ErrorCode::Ok;
    };
    auto sub = sess.onModuleChange("mobilenetwork", changeCb);
    //return true;
}
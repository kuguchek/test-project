#include "../hpp/NetConfAgent.hpp"

bool NetConfAgent::subscribeForModelChanges() {
    sess = con.sessionStart();
    sess.copyConfig(sysrepo::Datastore::Startup, "mobilenetwork");
    int32_t called = 0;

    sysrepo::ModuleChangeCb changeCb = [&called] (auto, auto, auto, auto, auto, auto) -> sysrepo::ErrorCode {
            called++;
            std::cout << "called: " << called << std::endl;
            return sysrepo::ErrorCode::Ok;
    };
    sub = sess.onModuleChange("mobilenetwork", changeCb);
    return true;
}
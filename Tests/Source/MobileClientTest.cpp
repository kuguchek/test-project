#include <gtest/gtest.h>
#include "MobileClient.hpp"
#include "NetConfAgentMock.hpp"
#include "Constants.hpp"

using testing::Return;
using testing::_;
using testing::StrictMock;
using testing::InSequence;
using testing::SetArgReferee;
using testing::DoAll;

namespace {
    const std::string pathPattern = "/commutator:subscribers/subscriber[number='";
    const std::string stateIdle = "idle";
    const std::string stateActive = "active";
    const std::string stateBusy = "busy";
    const std::string pathName = "']/userName";
    const std::string pathState = "']/state";
    const std::string pathIncoming = "']/incomingNumber";
    const std::string pathNumber = "']/number";
}

namespace test {
class MobileClientTest : public testing::Test {
    protected:
        void SetUp() override {
            auto tmpMock = std::make_unique<StrictMock<mock::NetConfAgentMock>>();
            _mock = tmpMock.get();
            _cli = std::make_unique<mobilenetwork::MobileClient>(std::move(tmpMock));
        }
        StrictMock <mock::NetConfAgentMock> *_mock;
        std::unique_ptr<mobilenetwork::MobileClient> _cli;
};

TEST_F(MobileClientTest, RegisterFail) 
{
    const std::string number = "0124";
    const std::string name = "testname";
    const std::string numberPath = pathPattern + number + pathNumber;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (true));
    EXPECT_FALSE(_cli->Register(number));
}

TEST_F(MobileClientTest, RegisterWithoutSetName) 
{
    const std::string number = "124";
    EXPECT_FALSE(_cli->Register(number));
}

TEST_F(MobileClientTest, RegisterUnregisterTwoTimes)
{
    InSequence s;
    const std::string number = "0456";
    const std::string name = "test";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath,_))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, deleteData(subscribePath))
        .Times(1);
    EXPECT_CALL(*_mock, closeSubscription())
        .Times(1);
    EXPECT_TRUE(_cli->unregister());
    EXPECT_FALSE(_cli->unregister());
}

TEST_F(MobileClientTest, CallBeforeRegister)
{
    EXPECT_FALSE(_cli->call("1234"));
}

TEST_F(MobileClientTest, CallSuccess)
{
    InSequence s;
    const std::string number = "0456";
    const std::string name = "test";
    const std::string income = "0124";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string incomePath = pathPattern + income + pathNumber;
    const std::string stateIncome = pathPattern + income + pathState;
    const std::string statePath = pathPattern + number + pathState;
    const std::string incomeNumber = pathPattern + income + pathIncoming;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath,_))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(incomePath,_))
        .WillOnce(Return (true));
    EXPECT_CALL(*_mock, fetchData(stateIncome,_))
        .WillOnce(DoAll(SetArgReferee<1>(stateIdle), Return(true)));
    EXPECT_CALL(*_mock, changeData(stateIncome, stateActive))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(statePath, stateActive))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(incomeNumber, number))
        .Times(1);
    EXPECT_TRUE(_cli->call(income));
}

TEST_F(MobileClientTest, AnswerSuccess)
{
    const std::string number = "0456";
    const std::string name = "test";
    const std::string income = "0321";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string statePath = pathPattern + number + pathState;
    const std::string incomePath = pathPattern + number + pathIncoming;
    const std::string namePathIncome = pathPattern + income + pathName;
    const std::string stateIncome = pathPattern + income + pathState;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath, _))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    _cli->handleModuleChange(statePath, stateActive);
    EXPECT_CALL(*_mock, fetchData(namePathIncome,_))
        .Times(1);
    _cli->handleModuleChange(incomePath, income);
    EXPECT_CALL(*_mock, changeData(statePath, stateBusy));
    EXPECT_CALL(*_mock, changeData(stateIncome, stateBusy));
    EXPECT_CALL(*_mock, notifySysrepo(_));
    EXPECT_TRUE(_cli->answer());
}

TEST_F(MobileClientTest, RegectSuccess)
{
    const std::string number = "0034";
    const std::string name = "name";
    const std::string income = "3400";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string statePath = pathPattern + number + pathState;
    const std::string incomePath = pathPattern + number + pathIncoming;
    const std::string namePathIncome = pathPattern + income + pathName;
    const std::string stateIncome = pathPattern + income + pathState;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath, _))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    _cli->handleModuleChange(statePath, stateActive);
    EXPECT_CALL(*_mock, fetchData(namePathIncome,_))
        .Times(1);
    _cli->handleModuleChange(incomePath, income);
    EXPECT_CALL(*_mock, changeData(incomePath, ""));
    EXPECT_CALL(*_mock, changeData(stateIncome, stateIdle));
    EXPECT_CALL(*_mock, changeData(statePath, stateIdle));
    EXPECT_TRUE(_cli->regect());
}

TEST_F(MobileClientTest, CallEndIncomingTrue)
{
    const std::string number = "0456";
    const std::string name = "test";
    const std::string income = "4321";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string statePath = pathPattern + number + pathState;
    const std::string namePathIncome = pathPattern + income + pathName;
    const std::string incomePath = pathPattern + number + pathIncoming;
    const std::string stateIncome = pathPattern + income + pathState;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath, _))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(namePathIncome,_))
        .Times(1);
    _cli->handleModuleChange(incomePath, income);
    _cli->handleModuleChange(statePath, stateBusy);
    EXPECT_CALL(*_mock, changeData(incomePath, ""))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(stateIncome, stateIdle))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(statePath, stateIdle))
        .Times(1);
    EXPECT_CALL(*_mock, notifySysrepo(_)).Times(1);
    EXPECT_TRUE(_cli->callEnd());
}

TEST_F(MobileClientTest, CallCallEndOutgoing)
{
    InSequence s;
    const std::string number = "0456";
    const std::string name = "test";
    const std::string income = "0124";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string incomePath = pathPattern + income + pathNumber;
    const std::string stateIncome = pathPattern + income + pathState;
    const std::string statePath = pathPattern + number + pathState;
    const std::string incomeNumber = pathPattern + income + pathIncoming;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath, _))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(incomePath,_))
        .WillOnce(Return (true));
    EXPECT_CALL(*_mock, fetchData(stateIncome,_))
        .WillOnce(DoAll(SetArgReferee<1>(stateIdle), Return(true)));
    EXPECT_CALL(*_mock, changeData(stateIncome, stateActive))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(statePath, stateActive))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(incomeNumber, number))
        .Times(1);
    EXPECT_TRUE(_cli->call(income));
    _cli->handleModuleChange(statePath, stateBusy);
    _cli->handleModuleChange(stateIncome, stateBusy);
    EXPECT_CALL(*_mock, changeData(incomeNumber, ""))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(stateIncome, stateIdle))
        .Times(1);
    EXPECT_CALL(*_mock, changeData(statePath, stateIdle))
        .Times(1);
    EXPECT_CALL(*_mock, notifySysrepo(_)).Times(1);
    EXPECT_TRUE(_cli->callEnd());
}

TEST_F(MobileClientTest, SetName) 
{
    const std::string empty;
    const std::string name = "testname";
    _cli->setName(name);
    EXPECT_EQ(name, _cli->getName());
    _cli->setName(empty);
    EXPECT_EQ(empty, _cli->getName());
}

TEST_F(MobileClientTest, NoSuchNumberCall) 
{
    InSequence s;
    const std::string name = "name";
    const std::string number = "12";
    const std::string income = "123";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string incomePath = pathPattern + income + pathNumber;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath, _))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(incomePath,_))
        .WillOnce(Return (false));
    EXPECT_FALSE(_cli->call(income));
}

TEST_F(MobileClientTest, AnswerRegectEndWithoutIncoming)
{
    const std::string name = "name";
    const std::string number = "12";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath,_))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_FALSE(_cli->answer());
    EXPECT_FALSE(_cli->regect());
    EXPECT_FALSE(_cli->callEnd());
}

TEST_F(MobileClientTest, CallSubscriberBusy)
{
    const std::string number = "342";
    const std::string name = "nametest";
    const std::string income = "0032";
    const std::string numberPath = pathPattern + number + pathNumber;
    const std::string subscribePath = pathPattern + number + "']";
    const std::string namePath = pathPattern + number + pathName;
    const std::string incomePath = pathPattern + income + pathNumber;
    const std::string stateIncome = pathPattern + income + pathState;

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(numberPath,_))
        .WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(numberPath,_))
        .Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(subscribePath,_))
        .Times(1);
    EXPECT_CALL(*_mock, registerOperData(namePath,_))
        .Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(incomePath,_))
        .WillOnce(Return (true));
    EXPECT_CALL(*_mock, fetchData(stateIncome,_))
        .WillOnce(DoAll(SetArgReferee<1>(stateBusy), Return(true)));
    EXPECT_FALSE(_cli->call(income));
}
}
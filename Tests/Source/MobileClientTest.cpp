#include "MobileClient.hpp"
#include "NetConfAgentMock.hpp"

#include <gtest/gtest.h>
#include "Constants.hpp"

using testing::Return;
using testing::_;
using testing::StrictMock;
using testing::InSequence;

namespace test {
class MobileClientTest : public testing::Test {
    protected:
        void SetUp() override {
            auto tmpMock = std::make_unique<StrictMock<NetConfAgentMock>>();
            _mock = tmpMock.get();
            _cli = std::make_unique<mobilenetwork::MobileClient>(std::move(tmpMock));
        }
        StrictMock <NetConfAgentMock> *_mock;
        std::unique_ptr<mobilenetwork::MobileClient> _cli;
};

TEST_F(MobileClientTest, SetName) 
{
    std::string name = "testname";
    _cli->setName(name);
    EXPECT_EQ(name, _cli->getName());
}

TEST_F(MobileClientTest, RegisterSuccess) 
{
    InSequence s;
    std::string number = "0123";
    std::string name = "testname";

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(_,_)).Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(_,_)).Times(1);
    EXPECT_CALL(*_mock, registerOperData(_,_)).Times(1);
    EXPECT_TRUE(_cli->Register(number));
}

TEST_F(MobileClientTest, RegisterFail) 
{
    std::string number = "0124";
    std::string name = "testname";

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (true));
    EXPECT_FALSE(_cli->Register(number));
}

TEST_F(MobileClientTest, RegisterUnregister)
{
    InSequence s;
    std::string number = "0456";
    std::string name = "test";
    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(_,_)).Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(_,_)).Times(1);
    EXPECT_CALL(*_mock, registerOperData(_,_)).Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, deleteData(_)).Times(1);
    EXPECT_CALL(*_mock, closeSubscription()).Times(1);
    EXPECT_TRUE(_cli->unregister());
}

TEST_F(MobileClientTest, RegisterUnregisterTwoTimes)
{
    InSequence s;
    std::string number = "0456";
    std::string name = "testname";
    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(_,_)).Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(_,_)).Times(1);
    EXPECT_CALL(*_mock, registerOperData(_,_)).Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, deleteData(_)).Times(1);
    EXPECT_CALL(*_mock, closeSubscription()).Times(1);
    EXPECT_TRUE(_cli->unregister());
    EXPECT_FALSE(_cli->unregister());
}

TEST_F(MobileClientTest, CallBeforeRegister)
{
    EXPECT_FALSE(_cli->call("1234"));
}

TEST_F(MobileClientTest, NoSuchNumberCall) 
{
    InSequence s;
    std::string name = "name";
    std::string number = "12";
    std::string income = "123";

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(_,_)).Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(_,_)).Times(1);
    EXPECT_CALL(*_mock, registerOperData(_,_)).Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_FALSE(_cli->call(income));
}

TEST_F(MobileClientTest, AnswerWithoutIncoming)
{
    InSequence s;
    std::string name = "name";
    std::string number = "12";

    _cli->setName(name);
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_CALL(*_mock, changeData(_,_)).Times(1);
    EXPECT_CALL(*_mock, subscribeForModelChanges(_,_)).Times(1);
    EXPECT_CALL(*_mock, registerOperData(_,_)).Times(1);
    EXPECT_TRUE(_cli->Register(number));
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_FALSE(_cli->answer());
}

TEST_F(MobileClientTest, RegectWithoutIncoming)
{
    EXPECT_CALL(*_mock, fetchData(_,_)).WillOnce(Return (false));
    EXPECT_FALSE(_cli->regect());
}


TEST_F(MobileClientTest, CallEndBeforeCall)
{
    EXPECT_FALSE(_cli->callEnd());
}
}
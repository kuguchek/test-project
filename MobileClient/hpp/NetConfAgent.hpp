# ifndef NETCONFAGENT_HPP
# define NETCONFAGENT_HPP
#include <sysrepo-cpp/Session.hpp>
#include <sysrepo-cpp/Connection.hpp>
#include <iostream>
#include <optional>
#include "INetConfAgent.hpp"

//class MobileClient;
namespace mobilenetwork {
class INetConfAgent;

class NetConfAgent : public INetConfAgent {
    public:
        /**
        * @brief Constructor for NetConfAgent class.
        */
        NetConfAgent();      
        /**
        * @brief Subscribe for changes made in the specified module
        * @param[in] path Path to subscribe for
        * @param[in] mc Reference to object to handle changes in MobileClient class
        */
        void subscribeForModelChanges(std::string const &path, MobileClient &mc) override;
        /**
        * @brief Get data at the given path(if node on given path has value)
        * @param[in] path Path to get data from
        * @param[out] str String representation of data
        * @return /true when there is data on specified path, /false otherwise.
        */
        bool fetchData(std::string const &path, std::string &str) override; //first const
        /**
        * @brief Set or change value at the given path
        * @param[in] path Path of the element to be changed
        * @param[in] value Value of the element to be changed
        */
        void changeData(std::string const &path, std::string const &value) override;
        /**
        * @brief Delete value at the given path
        * @param[in] path Path of the element to be deleted.
        */
        void deleteData(std::string path);
        /**
        * @brief Register for providing operational data at the given path. 
        * @param[in] path Path to subscribe for
        * @param[in] mc Reference to object to get data from MobileClient class
        */
        void registerOperData(const std::string &path, MobileClient &mc) override;
        /**
        * @brief Delete subscribtion for module changes
        */
        void closeSubscription() override;
        /**
        * @brief Destructor of NetConfAgent class
        */
        ~NetConfAgent();
    private:
        sysrepo::Connection _con;
        sysrepo::Session _ses;
        std::optional<sysrepo::Subscription> _sub;
        std::optional<sysrepo::Subscription> _subOper;
};
}
#endif
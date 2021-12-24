# ifndef INETCONFAGENT_HPP
# define INETCONFAGENT_HPP

#include <string>
#include <memory>
#include <map>

namespace mobilenetwork {
class MobileClient;

class INetConfAgent {
    public:
        /**
        * @brief Subscribe for changes made in the specified module
        * @param[in] path Path to subscribe for
        * @param[in] mc Reference to object to handle changes in MobileClient class
        */
        virtual void subscribeForModelChanges(std::string const &path, MobileClient &mc) = 0;
        /**
        * @brief Get data at the given path(if node on given path has value)
        * @param[in] path Path to get data from
        * @param[out] str String representation of data
        * @return /true when there is data on specified path, /false otherwise.
        */
        virtual bool fetchData(std::string const &path, std::string &str) = 0;
        /**
        * @brief Set or change value at the given path
        * @param[in] path Path of the element to be changed
        * @param[in] value Value of the element to be changed
        */
        virtual void changeData(std::string const &path, std::string const &value) = 0;
        /**
        * @brief Delete value at the given path
        * @param[in] path Path of the element to be deleted.
        */
        virtual void deleteData(std::string path) = 0;
        /**
        * @brief Register for providing operational data at the given path. 
        * @param[in] path Path to subscribe for
        * @param[in] mc Reference to object to get data from MobileClient class
        */
        virtual void registerOperData(const std::string &path, MobileClient &mc) = 0;
        /**
        * @brief Delete subscribtion for module changes
        */
        virtual void closeSubscription() = 0;
        /**
        * @brief Send notification to Sysrepo about start or end of the call
        * @param[in] map Map of pairs of pathes and values, to fill the 
        * notification about start of the call, subscribers and duration (when cal end)
        */
        virtual void notifySysrepo(std::map<std::string, std::string> map) = 0;
        /**
        * @brief Destructor of INetConfAgent class
        */
        virtual ~INetConfAgent() {};
};
}
#endif
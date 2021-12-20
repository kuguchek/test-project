# ifndef MOBILECLIENT_HPP
# define MOBILECLIENT_HPP

#include "INetConfAgent.hpp"
#include <memory>
#include <string>

namespace mobilenetwork {
class NetConfAgent;

enum class State {
    idle,
    busy,
    active
};

enum class Leaf {
    number,
    userName,
    incomingNumber,
    state,
    subscriber
};

class MobileClient {
    public:
        /**
        * @brief Constructor for MobileClient class.
        */
        MobileClient();
        MobileClient(std::unique_ptr<INetConfAgent> ptr);
        /**
        * @brief Set subscriber's name 
        * @param[in] name Name to set
        */
        void setName(std::string &name);
        /**
        * @brief Register a subscriber with given number
        * @param[in] number Subscriber's number 
        * @return /true if registrated succesfully, /false otherwise.
        */
        bool Register(std::string &number);
        /**
        * @brief Handle changes (subscribe for whitch was made on subscribeForModelChanges()),
        * depending on it path and changed value
        * @param[in] path Path of the changed value
        * @param[in] value Value of the changed element
        */
        void handleModuleChange(const std::string &path, const std::string &value);
        /**
        * @brief Call to another (already registered) subscriber
        * @param[in] incomingNumber Number to call
        * @return /false if one of two subscribers is not registered or if one of two in 
        * busy or active state /true otherwise.
        */
        bool call(std::string incomingNumber);
        /**
        * @brief Answer the incoming call, if subscriber have an incoming call from another  
        * subscriber, to start the conversation
        * @return /false, if subscriber has no incoming call to answer, /true othervise
        */
        bool answer();
        /**
        * @brief Regect the incoming call, if subscriber have an incoming call from another  
        * subscriber, before conversation started
        * @return /false, if subscriber has no incoming call, /true if there was incoming call
        * and now subscriber's states and incoming/outgoing number's get back to before-call 
        * condition
        */
        bool regect();
        /**
        * @brief End the call (incoming or outgoing)
        * @return /true if call was going before and now ended, /false if there is no call 
        * (incoming or outgoing)
        */
        bool callEnd();
        /**
        * @brief Unregister the subscriber and delete all connected data
        * @return /false if subscriber was not registered before, so his data can't be deleted, 
        * /true if unregistretion is succesfull
        */
        bool unregister();
        /**
        * @brief Get subscriber's name
        * @return Subscriber's name
        */
        std::string getName();
         /**
        * @brief Destructor of MobileClient class
        */
        ~MobileClient();
    private:
        /**
        * @brief Create path to the specific leaf, depending on the given number 
        * ('key' of 'subscriber' list) and type of the lea
        * @param[in] value Subscriber's number
        * @param[in] type Type of the leaf to whitch path is made
        * @return String with the path to the given leaf or presence container
        */
        std::string makePath(std::string &value, Leaf type);
        std::string _name;
        std::string _number;
        std::string _incomingNumber;
        std::string _outgoingNumber;
        State _state;
        std::unique_ptr<INetConfAgent> _netConf;
};
}
#endif
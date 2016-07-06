
#pragma once


#include <list>
#include <vector>
#include <unordered_map>
#include <memory>

#include <cppexpose/reflection/Object.h>

#include <gloperate/gloperate_api.h>

#include <gloperate/input/InputEvent.h>


namespace gloperate
{


class Environment;
class AbstractEventConsumer;
class AbstractMetaphor;
class AbstractDeviceProvider;
class AbstractDevice;
class Control;


/**
*  @brief
*    Manager for input device and consumers
*/
class GLOPERATE_API InputManager : public cppexpose::Object
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the manager belongs (must NOT be null!)
    */
    InputManager(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    ~InputManager();

    /**
    *  @brief
    *    Register an event consumer at the input manager
    *
    *  @param[in] consumer
    *    Event consumer (must NOT be null)
    */
    void registerConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Deregister an event consumer from the input manager
    *
    *  @param[in] consumer
    *    Event consumer (must NOT be null)
    */
    void deregisterConsumer(AbstractEventConsumer * consumer);

    /**
    *  @brief
    *    Add a device to the input manager
    *
    *  @param[in] device
    *    Input device (must NOT be null)
    */
    void addDevice(AbstractDevice * device);

    /**
    *  @brief
    *    Add a mapping to the input manager
    *
    *  @param[in] mapping
    *    mapping (must NOT be null)
    */
    void addMapping(unsigned int controlId, AbstractMetaphor * metaphor);

    /**
    *  @brief
    *    Forwards an Event to all registered Consumers
    *
    *  @param[in] event
    *    The Event to forward
    */
    void onEvent(const InputEvent & event);

    /**
    *  @brief
    *    Forwards an Event to all listening metaphors
    *
    * @param[in] control
    *    The dispatching control
    *
    *  @param[in] event
    *    The Event to forward
    */
    void onControlEvent(Control * control, const InputEvent & event);


protected:
    Environment                         * m_environment; ///< Gloperate environment to which the manager belongs
    std::list<AbstractEventConsumer *>  m_consumers;
    std::list<AbstractDeviceProvider *> m_deviceProviders;
    std::list<AbstractDevice *>         m_devices;
    std::list<InputEvent *>             m_events;
    std::unordered_multimap<unsigned int, AbstractMetaphor *> m_mapping;
};


} // namespace gloperate


#include <gloperate/input/InputManager.h>

#include <cassert>

#include <gloperate/input/AbstractDeviceProvider.h>
#include <gloperate/input/AbstractDevice.h>
#include <gloperate/input/AbstractEventConsumer.h>
#include <gloperate/input/Metaphor.h>
#include <gloperate/input/InputEvent.h>
#include <gloperate/input/Control.h>


namespace gloperate
{


InputManager::InputManager(Environment * environment)
: cppexpose::Object("input")
, m_environment(environment)
{
}

InputManager::~InputManager()
{
    for (const auto ptr : m_consumers)
    {
        delete ptr;
    }

    for (const auto ptr : m_deviceProviders)
    {
        delete ptr;
    }

    for (const auto ptr : m_devices)
    {
        delete ptr;
    }
}

void InputManager::registerConsumer(AbstractEventConsumer * consumer)
{
    assert(consumer != nullptr);
    m_consumers.push_back(consumer);
}

void InputManager::deregisterConsumer(AbstractEventConsumer * consumer)
{
    assert(consumer != nullptr);
    m_consumers.remove(consumer);
}

void InputManager::addDevice(AbstractDevice * device)
{
    assert(device != nullptr);
    m_devices.emplace_back(device);
}

void InputManager::addMapping(unsigned int controlId, AbstractMetaphor * metaphor)
{
    m_mapping.emplace(controlId, metaphor);
}

void InputManager::onEvent(const InputEvent & event)
{
    for (auto consumer : m_consumers)
    {
        consumer->onEvent(event);
    }
}

void InputManager::onControlEvent(Control * control, const InputEvent & event)
{
    auto range = m_mapping.equal_range(control->id());

    for(auto it = range.first; it != range.second; ++it)
    {
        it->second->onEvent(event);
    }
}


} // namespace gloperate

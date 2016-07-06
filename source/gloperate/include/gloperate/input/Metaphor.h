
#pragma once

#include <set>

#include <gloperate/input/InputEvent.h>
#include <gloperate/gloperate_api.h>

namespace gloperate{

class GLOPERATE_API AbstractMetaphor
{
public:
    virtual void onEvent(const InputEvent & event) = 0;

};

template <typename T>
class GLOPERATE_API Metaphor : public AbstractMetaphor
{
    using handlerType = void (T::*)(const InputEvent & event);

public:
    Metaphor(std::set<InputEvent::Type> types, handlerType handler, T * object);
    virtual void onEvent(const InputEvent & event) override;

private:
    handlerType m_handler;
    T * m_object;
    std::set<InputEvent::Type> m_types;
};

} // namespace gloperate

#include <gloperate/input/Metaphor.hpp>

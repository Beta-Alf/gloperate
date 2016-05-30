
#pragma once


#include <gloperate/pipeline/ProxyOutput.h>


namespace gloperate
{


template <typename T>
ProxyOutput<T>::ProxyOutput(const std::string & name, Stage * parent, const T & value)
: InputSlot<T>(value)
{
    // Do not add property to group yet, only initialize the property itself
    this->initProperty(name, nullptr, cppexpose::PropertyOwnership::None);

    // Register proxy output, will also add proxy output as a property
    this->initInputSlot(SlotType::ProxyOutput, parent, cppexpose::PropertyOwnership::None);
}

template <typename T>
ProxyOutput<T>::~ProxyOutput()
{
}


} // namespace gloperate

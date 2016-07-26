
#pragma once

#include <cppexpose/plugin/plugin_api.h>

#include <hidapi/hidapi.h>

#include <gloperate/input/AbstractDeviceProvider.h>

#include <gloperate-hidapi/gloperate-hidapi_api.h>
#include <gloperate/gloperate-version.h>

#include <memory>
#include <unordered_map>
#include <string>




namespace gloperate
{
    class InputManager;
    class AbstractDevice;
}

namespace gloperate_hidapi
{

class GLOPERATE_HIDAPI_API HIDDeviceProvider : public gloperate::AbstractDeviceProvider
{
    using DeviceMap = std::unordered_map<std::wstring,gloperate::AbstractDevice*>;

public:
    CPPEXPOSE_DECLARE_COMPONENT(
            HIDDeviceProvider, gloperate::AbstractDeviceProvider,
            "",
            "",
            "",
            "Detects and creates HID-devices",
            GLOPERATE_AUTHOR_ORGANIZATION,
            "v0.1.0")
    /**
     * @brief
     *   Constructor
     */
     HIDDeviceProvider();

    /**
     * @brief
     *   Destructor
     */
    virtual ~HIDDeviceProvider();

    /**
     * @brief
     *   Updates the list of active devices and polls from those still active
     */
    virtual void updateDevices() override;


private:

    void createDevice(hid_device_info * device, const std::wstring &deviceID);

    DeviceMap m_openDevices;
};

}

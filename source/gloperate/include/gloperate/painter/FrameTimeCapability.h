#pragma once

#include <gloperate/painter/AbstractCapability.h>

#include <gloperate/gloperate_api.h>

namespace gloperate
{
class GLOPERATE_API FrameTimeCapability : public AbstractCapability
{
public:
    void setTime(float time);
    float time();

protected:
    float m_time;
};

} // namespace gloperate

#include <gloperate/painter/FrameTimeCapability.h>

void gloperate::FrameTimeCapability::setTime(float time)
{
    m_time = time;
}

float gloperate::FrameTimeCapability::time()
{
    return m_time;
}

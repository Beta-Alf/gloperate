
#pragma once

#include <glm/vec3.hpp>

#include <globjects/base/ref_ptr.h>

#include <gloperate/painter/AbstractCapability.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Program;
    class Framebuffer;
    class Texture;
}

namespace gloperate
{

class GLOPERATE_API CameraCollisionCapability : public gloperate::AbstractCapability
{
public:
    CameraCollisionCapability();
    virtual ~CameraCollisionCapability();

    float getDistance(const glm::vec3& dir) const;

    void setCollisionFBO(globjects::ref_ptr<globjects::Framebuffer> collisionFBO, size_t texSize);

protected:
    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    size_t m_size;
};

} // namespace gloperate


#include <gloperate/painter/CameraCollisionCapability.h>

#include <limits>

#include <glm/gtc/matrix_transform.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>
#include <glbinding/gl/bitfield.h>
#include <glbinding/gl/types.h>

#include <globjects/Framebuffer.h>

using namespace gl;

namespace gloperate
{

CameraCollisionCapability::CameraCollisionCapability()
: m_fbo(nullptr)
, m_size(1024)
{

}

CameraCollisionCapability::~CameraCollisionCapability()
{

}

void CameraCollisionCapability::setCollisionFBO(globjects::ref_ptr<globjects::Framebuffer> collisionFBO, size_t texSize)
{
    m_fbo = collisionFBO;
    m_size = texSize;
}

float CameraCollisionCapability::getDistance(const glm::vec3 &dir) const
{
    if(!m_fbo)
    {
        return std::numeric_limits<float>::max();
    }

    float xAbs = std::abs(dir.x);
    float yAbs = std::abs(dir.y);
    float zAbs = std::abs(dir.z);
    float main, u, v;
    GLenum face;

    if(xAbs == 0 && yAbs == 0 && zAbs == 0)
    {
        return std::numeric_limits<float>::max();
    }

    // Find the correct axis to sample
    if(xAbs > yAbs && xAbs > zAbs)
    {
        face = GL_COLOR_ATTACHMENT0;
        main = dir.x; u = -dir.z; v = -dir.y;
    } else if(yAbs > zAbs)
    {
        auto sign = std::signbit(dir.y) ? -1 : 1;

        face = GL_COLOR_ATTACHMENT2;
        main = dir.y; u = dir.x; v = sign *dir.z;

    } else
    {
        face = GL_COLOR_ATTACHMENT4;
        main = dir.z; u = -dir.x; v = -dir.y;
    }

    // Find the correct orientation on this axis
    face = face + static_cast<int>(std::signbit(main));

    // Sample the coordinates
    glm::vec2 uv{u/std::abs(main), v/std::abs(main)};
    glm::ivec2 i_uv = uv * static_cast<float>(m_size/2)+static_cast<float>(m_size/2);
    std::vector<glm::vec3> buf(m_samplingSize*m_samplingSize);

    m_fbo->bind();

    std::array<GLint,4> sampleRange{i_uv.x,i_uv.y,m_samplingSize,m_samplingSize};
    m_fbo->readPixels(face, sampleRange,GL_DEPTH_COMPONENT,GL_FLOAT,buf.data());

    m_fbo->unbind();

    return buf[0].x;
}


} // namespace gloperate


#include <gloperate/navigation/FirstPersonNavigation.h>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/CameraCollisionCapability.h>

#include <glm/gtc/matrix_transform.hpp>

namespace{
    float INPUT_SCALING = 0.001f;

    float CONSTRAINT_ROT_MAX_V_UP = 0.001f;
    float CONSTRAINT_ROT_MAX_V_LO = 0.001f;

    float COLLISION_RADIUS = 0.2f;
}

namespace gloperate{

FirstPersonNavigation::FirstPersonNavigation(AbstractCameraCapability &cameraCapability, CameraCollisionCapability *collisionCapability)
: m_cameraCapability(cameraCapability)
, m_collisionCapability(collisionCapability)
{
}

void FirstPersonNavigation::moveAbsolute(glm::vec2 direction)
{
    glm::vec3 delta(direction.x, 0.f, direction.y);

    move(delta);
}

void FirstPersonNavigation::moveRelative(glm::vec2 direction)
{
    const glm::vec3 center = m_cameraCapability.center();
    const glm::vec3 eye = m_cameraCapability.eye();
    glm::vec3 forwards = glm::normalize(center - eye);
    glm::vec3 right = glm::normalize(glm::cross(forwards, m_cameraCapability.up()));

    glm::vec3 delta = forwards * direction.x + right * direction.y;

    move(delta);
}

void FirstPersonNavigation::move(const glm::vec3 & delta)
{
    float dist = getDistance(delta);

    if(dist - glm::length(delta) > COLLISION_RADIUS)
    {
        m_cameraCapability.setEye(m_cameraCapability.eye() + delta);
        m_cameraCapability.setCenter(m_cameraCapability.center() + delta);
    }
}

float FirstPersonNavigation::getDistance(const glm::vec3 &direction)
{
    if(!m_collisionCapability)
        return 0.0;
    else
        return m_collisionCapability->getDistance(direction);
}

void FirstPersonNavigation::rotate(glm::vec2 direction)
{
    const glm::vec3 center = m_cameraCapability.center();
    const glm::vec3 eye = m_cameraCapability.eye();
    const glm::vec3 ray(glm::normalize(center - eye));
    const glm::vec3 rotAxis(glm::cross(ray, m_cameraCapability.up()));

    direction *= INPUT_SCALING;

    //enforceRotationConstraints(direction.x, direction.y);

    glm::mat4x4 transform = glm::mat4x4();
    transform = glm::translate(transform, eye);
    transform = glm::rotate(transform, direction.x, m_cameraCapability.up());
    transform = glm::rotate(transform, direction.y, rotAxis);
    transform = glm::translate(transform, -eye);

    glm::vec4 newEye = transform * glm::vec4(eye, 1.0f);
    glm::vec4 newCenter = transform * glm::vec4(center, 1.0f);

    m_cameraCapability.setEye(glm::vec3(newEye));
    m_cameraCapability.setCenter(glm::vec3(newCenter));

    const glm::vec3 ray2(glm::normalize(center - eye));

    float dist = getDistance(ray2);

    globjects::debug() << dist;
}

void FirstPersonNavigation::enforceRotationConstraints(
    float & hAngle
,   float & vAngle) const
{
    // retrieve the angle between camera-center to up and test how much closer
    // to up/down it can be rotated and clamp if required.
    auto up = m_cameraCapability.up();
    auto viewDir = glm::normalize(m_cameraCapability.eye() - m_cameraCapability.center());

    auto va = acosf(glm::dot(viewDir, up));
    vAngle = glm::clamp(vAngle, CONSTRAINT_ROT_MAX_V_UP - va, CONSTRAINT_ROT_MAX_V_LO - va);
}

void FirstPersonNavigation::reset()
{
    m_cameraCapability.reset();
}

} // namespace gloperate

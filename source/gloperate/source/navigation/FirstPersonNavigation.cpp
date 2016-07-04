
#include <gloperate/navigation/FirstPersonNavigation.h>

#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/CameraCollisionCapability.h>

#include <glm/gtc/matrix_transform.hpp>

namespace{

    float CONSTRAINT_ROT_MAX_V_UP = 0.001f;
    float CONSTRAINT_ROT_MAX_V_LO = 0.001f;

    float COLLISION_RADIUS = 0.2f;
    float COLLISION_SAFETY = 0.005f;
}

namespace gloperate{

FirstPersonNavigation::FirstPersonNavigation(AbstractCameraCapability & cameraCapability, CameraCollisionCapability * collisionCapability)
: m_cameraCapability(cameraCapability)
, m_collisionCapability(collisionCapability)
{
}

void FirstPersonNavigation::moveRelative(const glm::vec3 & direction)
{
    move( glm::vec3( m_cameraCapability.viewInverted() * glm::vec4( direction, 0.0)));
}

void FirstPersonNavigation::move(glm::vec3 delta)
{
    enforceCollisionConstraint(delta);

    m_cameraCapability.setEye(m_cameraCapability.eye() + delta);
    m_cameraCapability.setCenter(m_cameraCapability.center() + delta);
}

float FirstPersonNavigation::getDistance(const glm::vec3 & direction)
{
    if(!m_collisionCapability)
        return 0.0;
    else
        return m_collisionCapability->getDistance(direction);
}

void FirstPersonNavigation::rotate(const glm::vec2 & direction)
{
    const glm::vec3 center = m_cameraCapability.center();
    const glm::vec3 eye = m_cameraCapability.eye();
    const glm::vec3 up = m_cameraCapability.up();
    const glm::vec3 yawAxis(glm::cross((center - eye), up));

    glm::mat4x4 transform = glm::mat4x4();
    transform = glm::translate(transform, eye);
    transform = glm::rotate(transform, direction.x, up);
    transform = glm::rotate(transform, direction.y, yawAxis);
    transform = glm::translate(transform, -eye);

    glm::vec4 newEye = transform * glm::vec4(eye, 1.0f);
    glm::vec4 newCenter = transform * glm::vec4(center, 1.0f);

    m_cameraCapability.setEye(glm::vec3(newEye));
    m_cameraCapability.setCenter(glm::vec3(newCenter));
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

void FirstPersonNavigation::enforceCollisionConstraint(glm::vec3 & delta)
{
    float dist = getDistance(delta);

    // use more samples
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 horizontal = glm::normalize(glm::cross(delta, delta+up));

    glm::vec3 samples[] =
    {
        delta+up*COLLISION_RADIUS,
        delta-up*COLLISION_RADIUS,
        delta+horizontal*COLLISION_RADIUS,
        delta-horizontal*COLLISION_RADIUS
    };

    for (auto& sample : samples)
        dist = std::min(dist, getDistance(sample));

    // if there is an imminent collision
    if(dist - glm::length(delta) < COLLISION_RADIUS)
    {
        // modify delta so as to be non-colliding
        auto scale = (dist - glm::length(delta) - COLLISION_SAFETY) / COLLISION_RADIUS;
        delta *= std::max(0.f, scale);
    }
}

void FirstPersonNavigation::reset()
{
    m_cameraCapability.reset();
}

} // namespace gloperate

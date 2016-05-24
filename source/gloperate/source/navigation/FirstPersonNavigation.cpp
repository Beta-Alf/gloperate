
#include <gloperate/navigation/FirstPersonNavigation.h>

#include <globjects/logging.h>

#include <gloperate/painter/AbstractCameraCapability.h>

#include <glm/gtc/matrix_transform.hpp>

namespace{
    float ROTATION_SCALING = 0.001f;

    float CONSTRAINT_ROT_MAX_V_UP = 0.001f;
    float CONSTRAINT_ROT_MAX_V_LO = 0.001f;
}

namespace gloperate{

FirstPersonNavigation::FirstPersonNavigation(gloperate::AbstractCameraCapability & cameraCapability)
: m_cameraCapability(cameraCapability)
{
}

void FirstPersonNavigation::move(const glm::vec2 & direction)
{
    glm::vec3 delta(direction.x, 0.f, direction.y);
    delta /= 1000.f;
    m_cameraCapability.setEye(m_cameraCapability.eye() + delta);
    m_cameraCapability.setCenter(m_cameraCapability.center() + delta);
}

void FirstPersonNavigation::rotate(glm::vec2 direction)
{
    const glm::vec3 center = m_cameraCapability.center();
    const glm::vec3 eye = m_cameraCapability.eye();
    const glm::vec3 ray(glm::normalize(center - eye));
    const glm::vec3 rotAxis(glm::cross(ray, m_cameraCapability.up()));

    direction *= ROTATION_SCALING;

    //enforceRotationConstraints(direction.x, direction.y);

    glm::mat4x4 transform = glm::mat4x4();
    transform = glm::translate(transform, center);
    transform = glm::rotate(transform, direction.x, m_cameraCapability.up());
    transform = glm::rotate(transform, -direction.y, rotAxis);
    transform = glm::translate(transform, -center);

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

} // namespace gloperate

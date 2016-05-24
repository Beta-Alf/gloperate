
#include <gloperate/navigation/FirstPersonNavigation.h>

#include <globjects/logging.h>

#include <gloperate/painter/AbstractCameraCapability.h>

#include <glm/gtc/matrix_transform.hpp>

namespace{
    float INPUT_SCALING = 0.001f;

    float CONSTRAINT_ROT_MAX_V_UP = 0.001f;
    float CONSTRAINT_ROT_MAX_V_LO = 0.001f;
}

namespace gloperate{

FirstPersonNavigation::FirstPersonNavigation(gloperate::AbstractCameraCapability & cameraCapability)
: m_cameraCapability(cameraCapability)
{
}

void FirstPersonNavigation::moveAbsolute(glm::vec2 direction)
{
    glm::vec3 delta(direction.x, 0.f, direction.y);
    m_cameraCapability.setEye(m_cameraCapability.eye() + delta);
    m_cameraCapability.setCenter(m_cameraCapability.center() + delta);
}

void FirstPersonNavigation::moveRelative(glm::vec2 direction)
{
    const glm::vec3 center = m_cameraCapability.center();
    const glm::vec3 eye = m_cameraCapability.eye();
    glm::vec3 forwards = glm::normalize(center - eye);
    glm::vec3 right = glm::normalize(glm::cross(forwards, m_cameraCapability.up()));

    glm::vec3 delta = forwards * direction.x + right * direction.y;

    m_cameraCapability.setEye(eye + delta);
    m_cameraCapability.setCenter(center + delta);
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

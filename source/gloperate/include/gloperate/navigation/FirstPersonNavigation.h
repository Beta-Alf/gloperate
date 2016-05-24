
#pragma once


#include <gloperate/ext-includes-begin.h>
#include <glm/glm.hpp>
#include <gloperate/ext-includes-end.h>

#include <gloperate/gloperate_api.h>

namespace gloperate
{

class AbstractCameraCapability;


/**
*  @brief
*    Camera navigation 'FirstPersonNavigation'
*
*    This interaction technique is based on the 'world in hand' metaphor
*    to control a virtual camera. It allows the user to pan, rotate,
*    and zoom, based on a reference point the camera currently looks at.
*    It is specialised to interact with Treemaps and can drive a CombinedProjectionCapability
*
*  @remarks
*    This interaction technique needs a depth buffer image to work
*    correctly, which is provided by the specified CoordinateProvider.
*/
class GLOPERATE_API FirstPersonNavigation
{
public:
    FirstPersonNavigation(gloperate::AbstractCameraCapability & cameraCapability);

    void move(const glm::vec2 & direction);
    void rotate(glm::vec2 direction);
    void enforceRotationConstraints(float & hAngle, float & vAngle) const;

protected:
    gloperate::AbstractCameraCapability & m_cameraCapability;
};

} // namespace gloperate

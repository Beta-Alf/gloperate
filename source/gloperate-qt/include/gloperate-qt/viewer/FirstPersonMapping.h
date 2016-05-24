
#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <gloperate-qt/viewer/AbstractQtMapping.h>

namespace gloperate
{
class KeyboardEvent;
class MouseEvent;
class WheelEvent;
class FirstPersonNavigation;

}

namespace gloperate_qt{

class GLOPERATE_QT_API FirstPersonMapping : public gloperate_qt::AbstractQtMapping
{

public:
    FirstPersonMapping(gloperate_qt::QtOpenGLWindow * window);
    virtual ~FirstPersonMapping();

    virtual void initializeTools() override;


protected:
    virtual void mapEvent(gloperate::AbstractEvent * event) override;

    void mapKeyboardEvent(gloperate::KeyboardEvent * event);
    void mapMouseEvent(gloperate::MouseEvent * event);
    void mapWheelEvent(gloperate::WheelEvent * event);

protected:
    std::unique_ptr<gloperate::FirstPersonNavigation> m_navigation;
    glm::ivec2 m_lastMousePosition;
    glm::ivec2 m_lastDelta;

    bool m_mouseCaught;

};

}


#include <gloperate-qt/viewer/FirstPersonMapping.h>
#include <gloperate/navigation/FirstPersonNavigation.h>

#include <gloperate/ext-includes-begin.h>
#include <QVector2D>
#include <gloperate/ext-includes-end.h>

#include <glbinding/gl/enum.h>

#include <gloperate/base/make_unique.hpp>
#include <gloperate/painter/AbstractCameraCapability.h>
#include <gloperate/painter/CameraCollisionCapability.h>
#include <gloperate/painter/FrameTimeCapability.h>
#include <gloperate/painter/Painter.h>
#include <gloperate/input/AbstractEvent.h>
#include <gloperate/input/KeyboardEvent.h>
#include <gloperate/input/MouseEvent.h>
#include <gloperate/input/WheelEvent.h>

#include <gloperate-qt/viewer/QtOpenGLWindow.h>


using namespace gloperate_qt;
using namespace gloperate;

using gloperate::make_unique;

namespace{
    float INPUT_SCALING = 0.001f;

    void resetMouse(QtOpenGLWindow * window)
    {
        QPoint middle = window->mapToGlobal(QPoint(window->width()/2,window->height()/2));

        QCursor::setPos(middle);
    }
}

namespace gloperate_qt{

FirstPersonMapping::FirstPersonMapping(QtOpenGLWindow * window)
: AbstractQtMapping(window)
, m_mouseCaught(false)
{
}

FirstPersonMapping::~FirstPersonMapping()
{
}

void FirstPersonMapping::initializeTools()
{
    if (!m_painter)
    {
        return;
    }

    if(!m_painter->supports<AbstractCameraCapability>())
        return;

    auto cameraCapability = m_painter->getCapability<AbstractCameraCapability>();
    auto collisionCapability = m_painter->getCapability<CameraCollisionCapability>();
    m_navigation = gloperate::make_unique<FirstPersonNavigation>(*cameraCapability, collisionCapability);

    m_timer = m_painter->getCapability<FrameTimeCapability>();
}

void FirstPersonMapping::mapEvent(AbstractEvent * event)
{
    if (!m_navigation)
    {
        return;
    }

    switch (event->sourceType())
    {
    case gloperate::EventType::Keyboard:
        mapKeyboardEvent(static_cast<KeyboardEvent *>(event));
        break;
    case gloperate::EventType::Mouse:
        mapMouseEvent(static_cast<MouseEvent *>(event));
        break;
    case gloperate::EventType::Wheel:
        mapWheelEvent(static_cast<WheelEvent *>(event));
        break;
    default:
        break;
    }
}

void FirstPersonMapping::mapKeyboardEvent(KeyboardEvent * event)
{
    if(!m_timer || !m_navigation)
    {
        return;
    }

    switch(event->key())
    {
    case gloperate::KeyW:
        m_navigation->moveRelative(glm::vec2(1.0, 0.0) * m_timer->time());
        break;
    case gloperate::KeyS:
        m_navigation->moveRelative(glm::vec2(-1.0, 0.0) * m_timer->time());
        break;
    case gloperate::KeyA:
        m_navigation->moveRelative(glm::vec2(0.0, -1.0) * m_timer->time());
        break;
    case gloperate::KeyD:
        m_navigation->moveRelative(glm::vec2(0.0, 1.0) * m_timer->time());
        break;
    case gloperate::KeyR:
        m_navigation->reset();
    default:
        break;
    }
}

void FirstPersonMapping::mapMouseEvent(MouseEvent * mouseEvent)
{
    if (mouseEvent && mouseEvent->type() == MouseEvent::Type::Move)
    {
        auto curPos = mouseEvent->pos();
        auto delta = m_lastMousePosition - curPos;
        m_lastMousePosition = curPos;

        if(m_mouseCaught)
        {
            m_lastMousePosition = glm::ivec2(m_window->width()/2,m_window->height()/2);
            resetMouse(m_window);
        }

        if(glm::length(glm::vec2(delta)) >= 1 )
            m_navigation->rotate(delta);
    }
    if(mouseEvent && mouseEvent->type() == MouseEvent::Type::Press)
    {
        switch(mouseEvent->button())
        {
        case MouseButtonLeft:
            m_mouseCaught = true;
            break;
        case MouseButtonRight:
            m_mouseCaught = false;
            break;
        default:
            break;
        }
    }
}

void FirstPersonMapping::mapWheelEvent(WheelEvent * wheelEvent)
{
    /*
    auto scale = wheelEvent->angleDelta().y;
    scale /= WheelEvent::defaultMouseAngleDelta();
    scale *= SCALE_FACTOR; // smoother (slower) scaling
    m_navigation->scaleAtMouse(wheelEvent->pos(), scale);
    */
}

} // namespace gloperate_qt

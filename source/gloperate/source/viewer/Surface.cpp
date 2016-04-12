
#include <globjects/base/baselogging.h>

#include <gloperate/viewer/Surface.h>

#include <gloperate/viewer/ViewerContext.h>


namespace gloperate
{


Surface::Surface(ViewerContext * viewerContext)
: reflectionzeug::Object("surface")
, m_viewerContext(viewerContext)
, m_openGLContext(nullptr)
{
    m_viewerContext->registerSurface(this);
}

Surface::~Surface()
{
    m_viewerContext->unregisterSurface(this);
}

ViewerContext * Surface::viewerContext() const
{
    return m_viewerContext;
}

AbstractGLContext * Surface::openGLContext() const
{
    return m_openGLContext;
}

void Surface::setOpenGLContext(AbstractGLContext * context)
{
    // Deinitialize renderer in old context
    if (m_openGLContext)
    {
        onContextDeinit();

        m_openGLContext = nullptr;
    }

    // Initialize renderer in new context
    if (context)
    {
        m_openGLContext = context;

        onContextInit();
    }
}

void Surface::onUpdate()
{
}

void Surface::onContextInit()
{
}

void Surface::onContextDeinit()
{
}

void Surface::onViewport(const glm::ivec4 &, const glm::ivec4 &)
{
}

void Surface::onBackgroundColor(float, float, float)
{
}

void Surface::onRender()
{
}

void Surface::onKeyPress(int, int)
{
}

void Surface::onKeyRelease(int, int)
{
}

void Surface::onMouseMove(const glm::ivec2 &)
{
}

void Surface::onMousePress(int, const glm::ivec2 &)
{
}

void Surface::onMouseRelease(int, const glm::ivec2 &)
{
}

void Surface::onMouseWheel(const glm::vec2 &, const glm::ivec2 &)
{
}

void Surface::createVideo()
{
    globjects::critical() << "createVideo() not implemented in Surface base class.";
}


} // namespace gloperate

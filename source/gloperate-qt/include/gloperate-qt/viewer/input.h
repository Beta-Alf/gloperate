
#pragma once

#include <QKeyEvent>
#include <QMouseEvent>

#include <glm/glm.hpp>

#include <gloperate/viewer/input.h>

#include <gloperate-qt/gloperate-qt_api.h>


namespace gloperate_qt
{


GLOPERATE_QT_API glm::ivec2 fromQPoint(const QPoint point);
GLOPERATE_QT_API gloperate::MouseButton fromQtMouseButton(Qt::MouseButton button);
GLOPERATE_QT_API gloperate::Key fromQtKeyCode(int key, int mods);


} // namespace gloperate_qt

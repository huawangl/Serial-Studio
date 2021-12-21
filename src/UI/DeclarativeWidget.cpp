/*
 * Copyright (c) 2020-2021 Alex Spataru <https://github.com/alex-spataru>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "DeclarativeWidget.h"
#include <Misc/ThemeManager.h>

namespace UI
{
/**
 * Creates a subclass of @c QWidget that allows us to call the given protected/private
 * @a function and pass the given @a event as a parameter to the @a function.
 */
#define EXEC_EVENT(pointer, function, event)                                             \
    if (!pointer.isNull())                                                               \
    {                                                                                    \
        class PwnedWidget : public QWidget                                               \
        {                                                                                \
        public:                                                                          \
            using QWidget::function;                                                     \
        };                                                                               \
        static_cast<PwnedWidget *>(pointer.data())->function(event);                     \
        update();                                                                        \
    }

/**
 * Configures item flags, sets fill color and connects signals/slots to automatically
 * resize the contained widget to the QML item's size.
 */
DeclarativeWidget::DeclarativeWidget(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setMipmap(true);
    setAntialiasing(true);
    setOpaquePainting(true);
    setAcceptTouchEvents(true);
    setFlag(ItemHasContents, true);
    setFlag(ItemIsFocusScope, true);
    setFlag(ItemAcceptsInputMethod, true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFillColor(Misc::ThemeManager::instance().base());

    // clang-format off
    connect(this, &QQuickPaintedItem::widthChanged,
            this, &UI::DeclarativeWidget::resizeWidget);
    connect(this, &QQuickPaintedItem::heightChanged,
            this, &UI::DeclarativeWidget::resizeWidget);
    connect(this, &UI::DeclarativeWidget::widgetChanged, [=](){update();});
    // clang-format on
}

/**
 * Returns a pointer to the contained widget
 */
QWidget *DeclarativeWidget::widget()
{
    return m_widget;
}

/**
 * Grabs an image/pixmap of the contained widget. The pixmap is later
 * used to render the widget in the QML interface without causing signal/slot
 * interferences with the scenegraph render thread.
 */
void DeclarativeWidget::update(const QRect &rect)
{
    if (widget())
    {
        m_pixmap = m_widget->grab();
        QQuickPaintedItem::update(rect);
    }
}

/**
 * Displays the pixmap generated in the @c update() function in the QML
 * interface through the given @a painter pointer.
 */
void DeclarativeWidget::paint(QPainter *painter)
{
    if (painter)
        painter->drawPixmap(0, 0, m_pixmap);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::keyPressEvent(QKeyEvent *event)
{
    EXEC_EVENT(m_widget, keyPressEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::keyReleaseEvent(QKeyEvent *event)
{
    EXEC_EVENT(m_widget, keyReleaseEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::inputMethodEvent(QInputMethodEvent *event)
{
    EXEC_EVENT(m_widget, inputMethodEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::focusInEvent(QFocusEvent *event)
{
    EXEC_EVENT(m_widget, focusInEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::focusOutEvent(QFocusEvent *event)
{
    EXEC_EVENT(m_widget, focusOutEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::mousePressEvent(QMouseEvent *event)
{
    EXEC_EVENT(m_widget, mousePressEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::mouseMoveEvent(QMouseEvent *event)
{
    EXEC_EVENT(m_widget, mouseMoveEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::mouseReleaseEvent(QMouseEvent *event)
{
    EXEC_EVENT(m_widget, mouseReleaseEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    EXEC_EVENT(m_widget, mouseDoubleClickEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::wheelEvent(QWheelEvent *event)
{
    EXEC_EVENT(m_widget, wheelEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::dragEnterEvent(QDragEnterEvent *event)
{
    EXEC_EVENT(m_widget, dragEnterEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::dragMoveEvent(QDragMoveEvent *event)
{
    EXEC_EVENT(m_widget, dragMoveEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::dragLeaveEvent(QDragLeaveEvent *event)
{
    EXEC_EVENT(m_widget, dragLeaveEvent, event);
}

/**
 * Passes the given @param event to the contained widget (if any).
 */
void DeclarativeWidget::dropEvent(QDropEvent *event)
{
    EXEC_EVENT(m_widget, dropEvent, event);
}

/**
 * Resizes the widget to fit inside the QML painted item.
 */
void DeclarativeWidget::resizeWidget()
{
    if (widget())
    {
        if (width() > 0 && height() > 0)
        {
            widget()->setFixedSize(width(), height());
            update();
        }
    }
}

/**
 * Changes the @param widget to be rendered in the QML interface.
 */
void DeclarativeWidget::setWidget(QWidget *widget)
{
    if (widget)
    {
        if (m_widget)
            delete m_widget;

        m_widget = widget;
        Q_EMIT widgetChanged();
    }
}
}

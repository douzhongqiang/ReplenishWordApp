#include "FreeTypeSelectedItem.h"
#include "FreeTypeConfig.h"
#include <QPainter>

FreeTypeSelectedItem::FreeTypeSelectedItem(QGraphicsItem* parentItem)
    :QGraphicsItem(parentItem)
{

}

FreeTypeSelectedItem::~FreeTypeSelectedItem()
{

}

void FreeTypeSelectedItem::setSelectedRect(const QRectF& rect)
{
    m_rect = rect;

    QPointF centerPos = m_rect.center();
    QPointF topLeftPos(centerPos.x() - m_rect.width() / 2, centerPos.y() - m_rect.height() / 2);
    m_rect2 = QRectF(QPointF(-m_rect.width() / 2, -m_rect.height() / 2), QSizeF(m_rect.width(), m_rect.height()));
    this->setPos(centerPos);

    this->update();
}

QRectF FreeTypeSelectedItem::getSelectedRect(void)
{
    return m_rect;
}

QRectF FreeTypeSelectedItem::boundingRect() const
{
    return m_rect2;
}

void FreeTypeSelectedItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    // Rects
    QVector<QRectF> handleRects;
    getHandleRects2(handleRects);

    painter->drawRect(m_rect2);

    // Draw Handles
    for (auto iter = handleRects.begin(); iter != handleRects.end(); ++iter)
    {
        painter->drawRect(*iter);
    }

    // Draw Rotate Handle
    qreal xPt = handleRects[1].center().x();
    qreal yPt = handleRects[1].top();
    QPointF topCenterPos(xPt, yPt);
    QPointF endCenterPos(xPt, yPt - m_nInterval2);
    painter->drawLine(topCenterPos, endCenterPos);

    // Draw Circle
    QPointF centerPos(xPt, yPt - m_nInterval2 - m_nInterval / 2);
    painter->drawEllipse(centerPos, m_nInterval / 2, m_nInterval / 2);
}

QPainterPath FreeTypeSelectedItem::shape() const
{
    QPainterPath path;
    path.addRect(m_rect);

    return path;
}

void FreeTypeSelectedItem::getHandleRects(QVector<QRectF>& rects)
{
    rects.clear();

    // Top Left
    rects << QRectF(m_rect.topLeft().x() - m_nInterval / 2.0, \
                    m_rect.topLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Top Middle
    rects << QRectF(m_rect.center().x() - m_nInterval / 2.0, \
                    m_rect.topLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Top Right
    rects << QRectF(m_rect.topRight().x() - m_nInterval / 2.0, \
                    m_rect.topRight().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Middle Left
    rects << QRectF(m_rect.topLeft().x() - m_nInterval / 2.0, \
                    m_rect.center().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Middle Right
    rects << QRectF(m_rect.topRight().x() - m_nInterval / 2.0, \
                    m_rect.center().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Left
    rects << QRectF(m_rect.bottomLeft().x() - m_nInterval / 2.0, \
                    m_rect.bottomLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Middle
    rects << QRectF(m_rect.center().x() - m_nInterval / 2.0, \
                    m_rect.bottomLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Right
    rects << QRectF(m_rect.bottomRight().x() - m_nInterval / 2.0, \
                    m_rect.bottomRight().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);
}

void FreeTypeSelectedItem::getHandleRects2(QVector<QRectF>& rects)
{
    rects.clear();

    // Top Left
    rects << QRectF(m_rect2.topLeft().x() - m_nInterval / 2.0, \
                    m_rect2.topLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Top Middle
    rects << QRectF(m_rect2.center().x() - m_nInterval / 2.0, \
                    m_rect2.topLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Top Right
    rects << QRectF(m_rect2.topRight().x() - m_nInterval / 2.0, \
                    m_rect2.topRight().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Middle Left
    rects << QRectF(m_rect2.topLeft().x() - m_nInterval / 2.0, \
                    m_rect2.center().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Middle Right
    rects << QRectF(m_rect2.topRight().x() - m_nInterval / 2.0, \
                    m_rect2.center().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Left
    rects << QRectF(m_rect2.bottomLeft().x() - m_nInterval / 2.0, \
                    m_rect2.bottomLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Middle
    rects << QRectF(m_rect2.center().x() - m_nInterval / 2.0, \
                    m_rect2.bottomLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Right
    rects << QRectF(m_rect2.bottomRight().x() - m_nInterval / 2.0, \
                    m_rect2.bottomRight().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);
}

// Get Rotate Rect
void FreeTypeSelectedItem::getRotateRect(QPointF& centerPos, qreal& r)
{
    // Rotate
    centerPos.setX(m_rect.center().x());
    centerPos.setY(m_rect.top() - m_nInterval / 2.0 - m_nInterval2 - m_nInterval / 2);

    r = m_nInterval / 2;
}

// set/get rotate
void FreeTypeSelectedItem::setRotate(qreal rotate)
{
    m_rotateValue = rotate;
    this->setRotation(m_rotateValue);
}

qreal FreeTypeSelectedItem::getRotate(void)
{
    return m_rotateValue;
}

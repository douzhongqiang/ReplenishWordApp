#include "FreeTypeGlyphItem.h"
#include <QDebug>

FreeTypeGlyphItem::FreeTypeGlyphItem(QGraphicsItem* parentItem)
    :QGraphicsItem(parentItem)
    , m_cSelectedColor(0, 0, 255)
    , m_cNormalColor(200, 200, 200)
{
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

FreeTypeGlyphItem::~FreeTypeGlyphItem()
{

}

void FreeTypeGlyphItem::setCurrentPointInfo(const FreeTypeCore::PointInfos& pointInfo)
{
    m_pointInfos = pointInfo;
    m_srcPointInfos = m_pointInfos;

    syncToPath();

    m_fixedPos = this->boundingRect().topLeft();
}

QRectF FreeTypeGlyphItem::boundingRect() const
{
    return m_path.boundingRect();
}

void FreeTypeGlyphItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen;
    if (this->isSelected())
        pen.setColor(m_cSelectedColor);
    else
        pen.setColor(m_cNormalColor);
    painter->setPen(pen);

    // Draw Path
    painter->drawPath(m_path);

    // Draw Selected
    if (this->isSelected())
    {
        // Draw Selected Rect
        painter->drawRect(m_path.boundingRect());

//        // Draw Selected Handle
//        QVector<QRectF> handleRects;
//        getHandleRects(handleRects);
//        painter->drawRects(handleRects);
    }
}

QPainterPath FreeTypeGlyphItem::shape() const
{
    QPainterPath path;
    path.addRect(m_path.boundingRect());

    return path;
}

QVariant FreeTypeGlyphItem::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change == QGraphicsItem::ItemSelectedChange)
        prepareGeometryChange();
    else if (change == QGraphicsItem::ItemPositionHasChanged)
    {
        emit signalItemPosChanged();
    }

    return QGraphicsItem::itemChange(change, value);
}

void FreeTypeGlyphItem::syncToPath(void)
{
    m_path.clear();

    int nLastDrawedIndex = 0;
    for (int i=0; i<m_pointInfos.size(); ++i)
    {
        if (i == 0 && m_pointInfos[i].pointType == 0)
        {
            m_path.moveTo(m_pointInfos[i].pos);
            nLastDrawedIndex = 0;
            continue;
        }

        if (m_pointInfos[i].pointType == 0)
        {
            if (i - 1 == nLastDrawedIndex)
                m_path.lineTo(m_pointInfos[i].pos);
            else if (i - 2 == nLastDrawedIndex)
            {
                m_path.quadTo(m_pointInfos[i - 1].pos, m_pointInfos[i].pos);
            }
            else if (i - 3 == nLastDrawedIndex)
            {
                m_path.cubicTo(m_pointInfos[i - 2].pos, m_pointInfos[i - 1].pos, m_pointInfos[i].pos);
            }

            nLastDrawedIndex = i;
        }

        if (i == m_pointInfos.size() - 1)
        {
            if (m_pointInfos[i].pointType == 0)
                m_path.lineTo(m_pointInfos[0].pos);
            else
            {
                if (i - 1 == nLastDrawedIndex)
                    m_path.quadTo(m_pointInfos[i].pos, m_pointInfos[0].pos);
                else if (i - 2 == nLastDrawedIndex)
                    m_path.cubicTo(m_pointInfos[i - 1].pos, m_pointInfos[i].pos, m_pointInfos[0].pos);
            }
        }
    }
}

void FreeTypeGlyphItem::setScaleValue(qreal xValue, qreal yValue)
{
    m_xScaleValue = xValue;
    m_yScaleValue = yValue;

    m_pointInfos.clear();
    for (auto iter = m_srcPointInfos.begin(); iter != m_srcPointInfos.end(); ++iter)
    {
        qreal xPt = iter->pos.x() * m_xScaleValue;
        qreal yPt = iter->pos.y() * m_yScaleValue;

        auto pointInfo = *iter;
        pointInfo.pos = QPointF(xPt, yPt);
        m_pointInfos << pointInfo;
    }

    // Set Scaled Pos
//    QPointF destPos;
//    destPos.setX(m_xScaleValue * this->pos().x());
//    destPos.setY(m_yScaleValue * this->pos().y());
//    this->setPos(this->pos() - destPos);

    syncToPath();

    // Set Dest Pos
    QRectF tempRectF = m_path.boundingRect();
    QPointF tempPos = tempRectF.topLeft() + this->pos() - m_fixedPos;
    this->blockSignals(true);
    this->setPos(this->pos() - tempPos);
    this->blockSignals(false);
}

void FreeTypeGlyphItem::getScaleValue(qreal& xValue, qreal& yValue)
{
    xValue = m_xScaleValue;
    yValue = m_yScaleValue;
}

void FreeTypeGlyphItem::setScaleFixedPos(const QPointF& pos)
{
    m_fixedPos = pos;
}

void FreeTypeGlyphItem::getHandleRects(QVector<QRectF>& rects)
{
    rects.clear();
    QRectF rect = this->boundingRect();

    // Top Left
    rects << QRectF(rect.topLeft().x() - m_nInterval / 2.0, \
                    rect.topLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Top Middle
    rects << QRectF(rect.center().x() - m_nInterval / 2.0, \
                    rect.topLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Top Right
    rects << QRectF(rect.topRight().x() - m_nInterval / 2.0, \
                    rect.topRight().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Middle Left
    rects << QRectF(rect.topLeft().x() - m_nInterval / 2.0, \
                    rect.center().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Middle Right
    rects << QRectF(rect.topRight().x() - m_nInterval / 2.0, \
                    rect.center().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Left
    rects << QRectF(rect.bottomLeft().x() - m_nInterval / 2.0, \
                    rect.bottomLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Middle
    rects << QRectF(rect.center().x() - m_nInterval / 2.0, \
                    rect.bottomLeft().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);

    // Bottom Right
    rects << QRectF(rect.bottomRight().x() - m_nInterval / 2.0, \
                    rect.bottomRight().y() - m_nInterval / 2.0, \
                    m_nInterval, m_nInterval);
}

#include "FreeTypeGlyphItem.h"

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
    syncToPath();
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

    painter->drawPath(m_path);

    if (this->isSelected())
        painter->drawRect(m_path.boundingRect());
}

QPainterPath FreeTypeGlyphItem::shape() const
{
    QPainterPath path;
    path.addRect(m_path.boundingRect());

    return path;
}

void FreeTypeGlyphItem::syncToPath(void)
{
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

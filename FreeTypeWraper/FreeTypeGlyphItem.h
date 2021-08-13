#ifndef FREETYPEGLYPHITEM_H
#define FREETYPEGLYPHITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainterPath>
#include "FreeTypeCore.h"

class FreeTypeGlyphItem : public QObject, public QGraphicsItem
{
    Q_OBJECT

public:
    FreeTypeGlyphItem(QGraphicsItem* parentItem = nullptr);
    ~FreeTypeGlyphItem();

    void setCurrentPointInfo(const FreeTypeCore::PointInfos& pointInfo);

    // Set/Get Scaled Vallue
    void setScaleValue(qreal xValue, qreal yValue);
    void getScaleValue(qreal& xValue, qreal& yValue);

    void setScaleFixedPos(const QPointF& pos);

public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) final;
    QPainterPath shape() const override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

private:
    FreeTypeCore::PointInfos m_srcPointInfos;
    FreeTypeCore::PointInfos m_pointInfos;
    QPainterPath m_path;

    void syncToPath(void);

    QColor m_cSelectedColor;
    QColor m_cNormalColor;

    qreal m_xScaleValue = 1.0;
    qreal m_yScaleValue = 1.0;

    int m_nInterval = 12;
    void getHandleRects(QVector<QRectF>& rects);

    QPointF m_fixedPos;

signals:
    void signalItemPosChanged(void);
};

#endif

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

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) final;
    QPainterPath shape() const override;

private:
    FreeTypeCore::PointInfos m_pointInfos;
    QPainterPath m_path;

    void syncToPath(void);

    QColor m_cSelectedColor;
    QColor m_cNormalColor;
};

#endif

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
    void getCurrentPointInfo(FreeTypeCore::PointInfos& pointInfos);

    // Set/Get Scaled Vallue
    void setScaleValue(qreal xValue, qreal yValue);
    void getScaleValue(qreal& xValue, qreal& yValue);

    void setScaleFixedPos(const QPointF& pos);

    // Handle About
    void setCurrentHandleIndex(int index);
    int getCurrentHandleIndex(void);
    bool getSelectLeftHandlePoint(QPointF& pos, int& index);
    bool getSelectRightHandlePoint(QPointF& pos, int& index);

    // Changed Point
    void changedPoint(int index, const QPointF& pos);
    // remove Point
    void removePoint(int index);
    // Add Copy Point
    void addCopyPoint(int index);

    void setIntervalPos(qreal xPt, qreal yPt);

    // Set/Get Rotate
    void setRotate(qreal rotate);
    qreal getRotate(void);

public:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) final;
    QPainterPath shape() const override;

    QVariant itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value) override;

    QRectF itemBoundingRect(void);

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

    // Draw Control And Points
    void drawControlPoints(QPainter* painter);
    int m_SelectedIndex = -1;
    void drawControlHandlePoints(QPainter* painter);

    // When can Spit To Two Item, This Spit
    void processSpitItem(void);

    // Convert To Normalize Points
    void converToNormalizePoint(const FreeTypeCore::PointInfos& pointInfo, FreeTypeCore::PointInfos& newPointInfo);

    qreal m_rotate = 0;

signals:
    void signalItemPosChanged(void);

private slots:
    void onHandleEnabledChanged(bool isEnabled);
    void oHandlePointSpitEnabledChanged(bool isEnabled);
};

#endif

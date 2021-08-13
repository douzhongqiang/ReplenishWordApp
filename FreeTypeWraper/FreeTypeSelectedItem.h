#ifndef FREETYPESELECTEDITEM_H
#define FREETYPESELECTEDITEM_H

#include <QObject>
#include <QGraphicsItem>

class FreeTypeSelectedItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    enum HandleType
    {
        t_TopLeftHandle,
        t_TopMiddleHandle,
        t_TopRightHandle,

        t_LeftMiddleHandle,
        t_RightMiddleHandle,

        t_BottomLeftHandle,
        t_BottomMiddleHandle,
        t_BottomRightHandle
    };

public:
    FreeTypeSelectedItem(QGraphicsItem* parentItem = nullptr);
    ~FreeTypeSelectedItem();

    // Get / Set Selected Rect
    void setSelectedRect(const QRectF& rect);
    QRectF getSelectedRect(void);

    // Get Handle Rects
    void getHandleRects(QVector<QRectF>& rects);

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) final;
    QPainterPath shape() const override;

private:
    QRectF m_rect;
    int m_nInterval = 12;
    int m_nInterval2 = 20;
};

#endif

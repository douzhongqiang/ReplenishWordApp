#ifndef FREETYPEOPERATOR_H
#define FREETYPEOPERATOR_H

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>

class FreeTypeRenderWidget;
class QGraphicsItem;

class FreeTypeOperatorBase
{
public:
    FreeTypeOperatorBase(FreeTypeRenderWidget* pRenderWidget);
    virtual ~FreeTypeOperatorBase();

    virtual void disposePressEvent(QMouseEvent* event);
    virtual void disposeMoveEvent(QMouseEvent* event);
    virtual void disposeReleaseEvent(QMouseEvent* event);
    virtual void disposeKeyPressEvent(QKeyEvent* event);
    virtual void disposeWheelEvent(QWheelEvent *event);

protected:
    FreeTypeRenderWidget* m_pRenderWidget = nullptr;
};

// ---------------------------------------------------------------------
// Def Oper
class FreeTypeDefOper : public FreeTypeOperatorBase
{
public:
    FreeTypeDefOper(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypeDefOper() override;

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;
    void disposeKeyPressEvent(QKeyEvent* event) override;
    void disposeWheelEvent(QWheelEvent *event) override;
};

// ---------------------------------------------------------------------
// Select Rect
class FreeTypeRectSelectOper : public FreeTypeOperatorBase
{
public:
    FreeTypeRectSelectOper(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypeRectSelectOper() override;

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

private:
    QPoint m_pos;
};

// ---------------------------------------------------------------------
// Select Move Operator
class FreeTypeMoveSelectedOper : public FreeTypeOperatorBase
{
public:
    FreeTypeMoveSelectedOper(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypeMoveSelectedOper();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

private:
    QPointF m_scenePos;
    QList<QGraphicsItem *> m_items;

    QVector<qreal> m_xValues;
    QVector<qreal> m_yValues;

    QVector<qreal> m_xOldValues;
    QVector<qreal> m_yOldValues;
};

#endif

#ifndef FREETYPEOPERATOR_H
#define FREETYPEOPERATOR_H

#include <QObject>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QWheelEvent>
#include "FreeTypeSelectedItem.h"

class FreeTypeRenderWidget;
class QGraphicsItem;
class FreeTypeGlyphItem;

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

// ---------------------------------------------------------------------
// Point Select Operator
class FreeTypePointSelectOper : public FreeTypeOperatorBase
{
public:
    FreeTypePointSelectOper(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypePointSelectOper();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;
};

// ---------------------------------------------------------------------
// Shift Point Select Operator
class FreeTypeShiftSelectMoveOper : public FreeTypeOperatorBase
{
public:
    FreeTypeShiftSelectMoveOper(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypeShiftSelectMoveOper();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

private:
    QPointF m_scenePos;
    QList<QGraphicsItem *> m_items;
    int m_hasAdjusted = 0;      // 0: None, 1: X , 2: Y
    int m_nCounter = 0;

    QVector<qreal> m_xValues;
    QVector<qreal> m_yValues;
};

// ---------------------------------------------------------------------
// Select Handle Operator
class FreeTypeHandleOperator : public FreeTypeOperatorBase
{
public:
    FreeTypeHandleOperator(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypeHandleOperator();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

    static bool needDisposeThisOper(FreeTypeRenderWidget* pRenderWidget, QMouseEvent* event, int& type);
    static void processMouseCursor(FreeTypeRenderWidget* pRenderWidget, int type);

    void setCurrentHandleType(FreeTypeSelectedItem::HandleType handleType);
    void setShiftPressed(bool isVisible);

private:
    QPoint m_pos;
    QPointF m_scenePos;

    qreal m_startXScale = 1.0;
    qreal m_startyScale = 1.0;

    qreal m_nSelectRectWidth = 1.0;
    qreal m_nSelectRectHeight = 1.0;

    FreeTypeSelectedItem::HandleType m_handleType;
    QRectF calcResultRect(const QRectF& rect, qreal xInterval, qreal yInterval);
    void scaleTranslateGlyphItems(qreal xScaleValue, qreal yScaleValue);

    QMap<FreeTypeGlyphItem*, QVector2D> m_scaleValueMaps;
    QMap<FreeTypeGlyphItem*, QVector2D> m_fixedPosFactorMaps;

    bool m_isShiftPressed = false;
};

// ---------------------------------------------------------------------
// Point Select Operator
class FreeTypePointHandleOperator : public FreeTypeOperatorBase
{
public:
    FreeTypePointHandleOperator(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypePointHandleOperator();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

    static bool needDisposeThisOper(FreeTypeRenderWidget* pRenderWidget, QMouseEvent* event, \
                                    FreeTypeGlyphItem*& pItem, int& index, int& pointType);

    void initCurrentInfo(int index, FreeTypeGlyphItem* pItem, int pointType);

private:
    int m_nCurrentSelectedIndex = -1;
    FreeTypeGlyphItem* m_pGlyphItem = nullptr;
    int m_operatorPointType = 0;
};

// ---------------------------------------------------------------------
// Point Delete Operator
class FreeTypePointHandleDeleteOperator : public FreeTypeOperatorBase
{
public:
    FreeTypePointHandleDeleteOperator(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypePointHandleDeleteOperator();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

    void initCurrentInfo(int index, FreeTypeGlyphItem* pItem);

private:
    int m_nCurrentSelectedIndex = -1;
    FreeTypeGlyphItem* m_pGlyphItem = nullptr;
};

// ---------------------------------------------------------------------
// Point Spit Operator
class FreeTypePointHandleSpitOperator : public FreeTypeOperatorBase
{
public:
    FreeTypePointHandleSpitOperator(FreeTypeRenderWidget* pRenderWidget);
    ~FreeTypePointHandleSpitOperator();

    void disposePressEvent(QMouseEvent* event) override;
    void disposeMoveEvent(QMouseEvent* event) override;
    void disposeReleaseEvent(QMouseEvent* event) override;

    void initCurrentInfo(int index, FreeTypeGlyphItem* pItem);

private:
    int m_nCurrentSelectedIndex = -1;
    FreeTypeGlyphItem* m_pGlyphItem = nullptr;
};

#endif

#include "FreeTypeOperator.h"
#include "FreeTypeRenderWidget.h"
#include <QGraphicsItem>

FreeTypeOperatorBase::FreeTypeOperatorBase(FreeTypeRenderWidget* pRenderWidget)
    :m_pRenderWidget(pRenderWidget)
{

}

FreeTypeOperatorBase::~FreeTypeOperatorBase()
{

}

void FreeTypeOperatorBase::disposePressEvent(QMouseEvent* event)
{

}

void FreeTypeOperatorBase::disposeMoveEvent(QMouseEvent* event)
{

}

void FreeTypeOperatorBase::disposeReleaseEvent(QMouseEvent* event)
{

}

void FreeTypeOperatorBase::disposeKeyPressEvent(QKeyEvent* event)
{

}

void FreeTypeOperatorBase::disposeWheelEvent(QWheelEvent *event)
{

}

// ---------------------------------------------------------------------
// Def Oper
FreeTypeDefOper::FreeTypeDefOper(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypeDefOper::~FreeTypeDefOper()
{

}

void FreeTypeDefOper::disposePressEvent(QMouseEvent* event)
{
    auto selectedItems = m_pRenderWidget->scene()->selectedItems();
    QPointF scenePos = m_pRenderWidget->mapToScene(event->pos());
    auto items = m_pRenderWidget->scene()->items(scenePos);

    // PointItem Has Selected Or Not
    bool hasSelected = false;
    foreach (auto item, items)
    {
        if (item->isSelected())
        {
            hasSelected = true;
            break;
        }
    }
    bool hasEnteredShiftKey = event->modifiers() & Qt::ShiftModifier;

    // Selected
    if (selectedItems.size() >= 1 && items.size() > 0 && hasSelected && !hasEnteredShiftKey)
    {
        // Set FreeTypeMoveSelectedOper
        FreeTypeMoveSelectedOper* pMoveSelectedOperator = new FreeTypeMoveSelectedOper(m_pRenderWidget);
        m_pRenderWidget->setOperator(pMoveSelectedOperator);
        pMoveSelectedOperator->disposePressEvent(event);
    }
    else if (selectedItems.size() >= 1 && items.size() > 0 && hasSelected && hasEnteredShiftKey)
    {
        // Set FreeTypeMoveSelectedOper
        FreeTypeShiftSelectMoveOper* pMoveSelectedOperator = new FreeTypeShiftSelectMoveOper(m_pRenderWidget);
        m_pRenderWidget->setOperator(pMoveSelectedOperator);
        pMoveSelectedOperator->disposePressEvent(event);
    }
    else if (items.size() > 0)
    {
        // Select Oper
        FreeTypePointSelectOper* pPointSelectOper = new FreeTypePointSelectOper(m_pRenderWidget);
        m_pRenderWidget->setOperator(pPointSelectOper);
        pPointSelectOper->disposePressEvent(event);
    }
    else
    {
        // Rect Select Oper
        FreeTypeRectSelectOper* pRectSelectOperator = new FreeTypeRectSelectOper(m_pRenderWidget);
        m_pRenderWidget->setOperator(pRectSelectOperator);
        pRectSelectOperator->disposePressEvent(event);
    }
}

void FreeTypeDefOper::disposeMoveEvent(QMouseEvent* event)
{

}

void FreeTypeDefOper::disposeReleaseEvent(QMouseEvent* event)
{

}

void FreeTypeDefOper::disposeKeyPressEvent(QKeyEvent* event)
{

}

void FreeTypeDefOper::disposeWheelEvent(QWheelEvent *event)
{

}

// ---------------------------------------------------------------------
// Select Rect
FreeTypeRectSelectOper::FreeTypeRectSelectOper(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypeRectSelectOper::~FreeTypeRectSelectOper()
{

}

void FreeTypeRectSelectOper::disposePressEvent(QMouseEvent* event)
{
    m_pos = event->pos();

    QRect nRect(0, 0, 0, 0);
    m_pRenderWidget->setSelectRect(nRect);

    auto items = m_pRenderWidget->scene()->selectedItems();
    foreach(auto item, items)
        item->setSelected(false);

    m_pRenderWidget->setSelectRectEnabled(true);
}

void FreeTypeRectSelectOper::disposeMoveEvent(QMouseEvent* event)
{
    QPoint pos = event->pos();

    int xPt = qMin(m_pos.x(), pos.x());
    int yPt = qMin(m_pos.y(), pos.y());
    int nWidth = abs(m_pos.x() - pos.x());
    int nHeight = abs(m_pos.y() - pos.y());

    QRect nRect(xPt, yPt, nWidth, nHeight);
    m_pRenderWidget->setSelectRect(nRect);
}

void FreeTypeRectSelectOper::disposeReleaseEvent(QMouseEvent* event)
{
    QPointF endPos = m_pRenderWidget->mapToScene(event->pos());
    QPointF pressedScenePos = m_pRenderWidget->mapToScene(m_pos);

    // Get Scene Rect
    qreal xStart = qMin(pressedScenePos.x(), endPos.x());
    qreal xEnd = qMax(pressedScenePos.x(), endPos.x());
    qreal yStart = qMin(pressedScenePos.y(), endPos.y());
    qreal yEnd = qMax(pressedScenePos.y(), endPos.y());

    // Set Item Selected
    QRectF rect(QPointF(xStart, yStart), QPointF(xEnd, yEnd));
    QList<QGraphicsItem *> items = m_pRenderWidget->scene()->items(rect, Qt::IntersectsItemShape);
    foreach(auto item, items)
        item->setSelected(true);

    m_pRenderWidget->setSelectRectEnabled(false);

    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

// ---------------------------------------------------------------------
// Select Move Operator
FreeTypeMoveSelectedOper::FreeTypeMoveSelectedOper(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypeMoveSelectedOper::~FreeTypeMoveSelectedOper()
{

}

void FreeTypeMoveSelectedOper::disposePressEvent(QMouseEvent* event)
{
    m_items = m_pRenderWidget->scene()->selectedItems();
    m_scenePos = m_pRenderWidget->mapToScene(event->pos());

    foreach (auto item, m_items)
    {
        QGraphicsItem* pItem = qgraphicsitem_cast<QGraphicsItem*>(item);


        m_xValues << pItem->pos().x();
        m_yValues << pItem->pos().y();
    }

    m_xOldValues = m_xValues;
    m_yOldValues = m_yValues;
}

void FreeTypeMoveSelectedOper::disposeMoveEvent(QMouseEvent* event)
{
    QPointF pos = m_pRenderWidget->mapToScene(event->pos());
    qreal xIntervalue = pos.x() - m_scenePos.x();
    qreal yIntervalue = pos.y() - m_scenePos.y();
    m_scenePos = pos;

    int count = 0;
    foreach (auto item, m_items)
    {
        QPointF itemPos = item->pos();
        itemPos.setX(itemPos.x() + xIntervalue);
        itemPos.setY(itemPos.y() + yIntervalue);

        item->setPos(itemPos);

        m_xValues[count] = itemPos.x();
        m_yValues[count++] = itemPos.y();
    }
}

void FreeTypeMoveSelectedOper::disposeReleaseEvent(QMouseEvent* event)
{
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

// ---------------------------------------------------------------------
// Point Select Operator
FreeTypePointSelectOper::FreeTypePointSelectOper(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypePointSelectOper::~FreeTypePointSelectOper()
{

}

void FreeTypePointSelectOper::disposePressEvent(QMouseEvent* event)
{
    bool hasEnteredShiftKey = event->modifiers() & Qt::ShiftModifier;

    if (!hasEnteredShiftKey)
    {
        // Clear All Select
        auto selectedItems = m_pRenderWidget->scene()->selectedItems();
        for (auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
            (*iter)->setSelected(false);
    }

    auto items = m_pRenderWidget->items(event->pos());

    for (auto iter = items.begin(); iter != items.end(); ++iter)
    {
        if (!hasEnteredShiftKey)
            (*iter)->setSelected(true);
        else
            (*iter)->setSelected(!(*iter)->isSelected());
    }
}

void FreeTypePointSelectOper::disposeMoveEvent(QMouseEvent* event)
{

}

void FreeTypePointSelectOper::disposeReleaseEvent(QMouseEvent* event)
{
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

// ---------------------------------------------------------------------
// Shift Point Select Operator
FreeTypeShiftSelectMoveOper::FreeTypeShiftSelectMoveOper(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypeShiftSelectMoveOper::~FreeTypeShiftSelectMoveOper()
{

}

void FreeTypeShiftSelectMoveOper::disposePressEvent(QMouseEvent* event)
{
    m_items = m_pRenderWidget->scene()->selectedItems();
    m_scenePos = m_pRenderWidget->mapToScene(event->pos());

    foreach (auto item, m_items)
    {
        QGraphicsItem* pItem = qgraphicsitem_cast<QGraphicsItem*>(item);

        m_xValues << pItem->pos().x();
        m_yValues << pItem->pos().y();
    }
}

void FreeTypeShiftSelectMoveOper::disposeMoveEvent(QMouseEvent* event)
{
    QPointF pos = m_pRenderWidget->mapToScene(event->pos());
    qreal xIntervalue = pos.x() - m_scenePos.x();
    qreal yIntervalue = pos.y() - m_scenePos.y();

    if (m_hasAdjusted == 0)
    {
        if (xIntervalue > yIntervalue)
            m_hasAdjusted = 1;
        else
            m_hasAdjusted = 2;
    }

    if (m_hasAdjusted == 1)
        yIntervalue = 0;
    else
        xIntervalue = 0;

    m_scenePos = pos;

    int count = 0;
    foreach (auto item, m_items)
    {
        QPointF itemPos = item->pos();
        itemPos.setX(itemPos.x() + xIntervalue);
        itemPos.setY(itemPos.y() + yIntervalue);

        item->setPos(itemPos);

        m_xValues[count] = itemPos.x();
        m_yValues[count++] = itemPos.y();
    }
}

void FreeTypeShiftSelectMoveOper::disposeReleaseEvent(QMouseEvent* event)
{
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}


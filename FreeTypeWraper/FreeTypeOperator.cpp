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

    if (selectedItems.size() >= 1 && items.size() > 0)
    {
//        bool hasSelected = false;
//        foreach (auto item, items)
//        {
//            if (item->isSelected())
//            {
//                hasSelected = true;
//                break;
//            }
//        }
//        if (!hasSelected)
//            return;

        // Set FreeTypeMoveSelectedOper
        FreeTypeMoveSelectedOper* pMoveSelectedOperator = new FreeTypeMoveSelectedOper(m_pRenderWidget);
        m_pRenderWidget->setOperator(pMoveSelectedOperator);
        pMoveSelectedOperator->disposePressEvent(event);
        return;
    }

    FreeTypeRectSelectOper* pRectSelectOperator = new FreeTypeRectSelectOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pRectSelectOperator);
    pRectSelectOperator->disposePressEvent(event);
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

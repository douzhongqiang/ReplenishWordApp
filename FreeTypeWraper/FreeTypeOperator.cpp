#include "FreeTypeOperator.h"
#include "FreeTypeRenderWidget.h"
#include "FreeTypeSelectedItem.h"
#include "FreeTypeGlyphItem.h"
#include "FreeTypeTool.h"
#include "FreeTypeConfig.h"
#include <QGraphicsItem>
#include <QDebug>

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

    bool hasEnteredShiftKey = event->modifiers() & Qt::ShiftModifier;

    // For Point Handle Select
    FreeTypeGlyphItem* pGlyphItem = nullptr;
    int nIndex = -1, nPointType = 0;
    bool result = FreeTypePointHandleOperator::needDisposeThisOper(m_pRenderWidget, event, pGlyphItem, nIndex, nPointType);
    if (result && g_FreeTypeConfig->isHandleEnabled())
    {
        if (g_FreeTypeConfig->isHandlePointDeleteMode() && nPointType == 0)
        {
            FreeTypePointHandleDeleteOperator* pDeletePointHandleOperator = new FreeTypePointHandleDeleteOperator(m_pRenderWidget);
            m_pRenderWidget->setOperator(pDeletePointHandleOperator);
            pDeletePointHandleOperator->initCurrentInfo(nIndex, pGlyphItem);
            pDeletePointHandleOperator->disposePressEvent(event);
        }
        else if (g_FreeTypeConfig->isHandlePointSpitEnabled() && nPointType == 0)
        {
            FreeTypePointHandleSpitOperator* pSpitPointHandleOperator = new FreeTypePointHandleSpitOperator(m_pRenderWidget);
            m_pRenderWidget->setOperator(pSpitPointHandleOperator);
            pSpitPointHandleOperator->initCurrentInfo(nIndex, pGlyphItem);
            pSpitPointHandleOperator->disposePressEvent(event);
        }
        else
        {
            FreeTypePointHandleOperator* pPointHandleOperator = new FreeTypePointHandleOperator(m_pRenderWidget);
            m_pRenderWidget->setOperator(pPointHandleOperator);
            pPointHandleOperator->initCurrentInfo(nIndex, pGlyphItem, nPointType);
            pPointHandleOperator->disposePressEvent(event);
        }

        return;
    }

    int type = 0;
    result = FreeTypeHandleOperator::needDisposeThisOper(m_pRenderWidget, event, type);
    if (result)
    {
        // Set FreeTypeMoveSelectedOper
        FreeTypeHandleOperator* pHandleOperator = new FreeTypeHandleOperator(m_pRenderWidget);
        m_pRenderWidget->setOperator(pHandleOperator);
        pHandleOperator->setCurrentHandleType((FreeTypeSelectedItem::HandleType)type);
        pHandleOperator->setShiftPressed(hasEnteredShiftKey);
        pHandleOperator->disposePressEvent(event);
        return;
    }

    // Selected
    if (items.size() > 0)
    {
        if (!hasEnteredShiftKey)
        {
            // Set FreeTypeMoveSelectedOper
            FreeTypeMoveSelectedOper* pMoveSelectedOperator = new FreeTypeMoveSelectedOper(m_pRenderWidget);
            m_pRenderWidget->setOperator(pMoveSelectedOperator);
            pMoveSelectedOperator->disposePressEvent(event);
        }
        else
        {
            // Set FreeTypeMoveSelectedOper
            FreeTypeShiftSelectMoveOper* pMoveSelectedOperator = new FreeTypeShiftSelectMoveOper(m_pRenderWidget);
            m_pRenderWidget->setOperator(pMoveSelectedOperator);
            pMoveSelectedOperator->disposePressEvent(event);
        }

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
    if (g_FreeTypeConfig->isHandleEnabled())
    {
        // On Handle To Mouse Cursor
        FreeTypeGlyphItem* pGlyphItem = nullptr;
        int index = -1, pointType = 0;
        bool result = FreeTypePointHandleOperator::needDisposeThisOper(m_pRenderWidget, event, pGlyphItem, index, pointType);
        if (result)
            m_pRenderWidget->viewport()->setCursor(Qt::PointingHandCursor);
        else
            m_pRenderWidget->viewport()->setCursor(Qt::ArrowCursor);
        return;
    }


    int type = 0;
    bool result = FreeTypeHandleOperator::needDisposeThisOper(m_pRenderWidget, event, type);
    if (!result)
    {
        m_pRenderWidget->viewport()->setCursor(Qt::ArrowCursor);
        return;
    }

    FreeTypeHandleOperator::processMouseCursor(m_pRenderWidget, type);
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
    m_scenePos = m_pRenderWidget->mapToScene(event->pos());

    auto items = m_pRenderWidget->items(event->pos());

    // Selected Current Point
    bool hasSelected = false;
    for (auto iter = items.begin(); iter != items.end(); ++iter)
    {
        if ((*iter)->isSelected())
            hasSelected = true;
    }

    // Clear All Select
    if (!hasSelected)
    {
        auto selectedItems = m_pRenderWidget->scene()->selectedItems();
        for (auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
            (*iter)->setSelected(false);
    }

    // Select Current Point
    for (auto iter = items.begin(); iter != items.end(); ++iter)
        (*iter)->setSelected(true);

    m_items = m_pRenderWidget->scene()->selectedItems();
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
    m_scenePos = m_pRenderWidget->mapToScene(event->pos());

    // Set Current Selected
    auto items = m_pRenderWidget->items(event->pos());
    for (auto iter = items.begin(); iter != items.end(); ++iter)
        (*iter)->setSelected(!(*iter)->isSelected());

    m_items = m_pRenderWidget->scene()->selectedItems();

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

    if (m_nCounter <= 5)
    {
        m_nCounter++;
        return;
    }

    m_nCounter = 0;
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

// ---------------------------------------------------------------------
// Select Handle Operator
FreeTypeHandleOperator::FreeTypeHandleOperator(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypeHandleOperator::~FreeTypeHandleOperator()
{

}

bool FreeTypeHandleOperator::needDisposeThisOper(FreeTypeRenderWidget* pRenderWidget, QMouseEvent* event, int& type)
{
    auto pSelectItem = pRenderWidget->getScaledItemHandleItem();
    if (!pSelectItem)
        return false;

    QPointF scenePos = pRenderWidget->mapToScene(event->pos());
    QVector<QRectF> handleRects;
    pSelectItem->getHandleRects(handleRects);

    for (int i=0; i<handleRects.size(); ++i)
    {
        if (handleRects[i].contains(scenePos))
        {
            type = i;
            return true;
        }
    }

    return false;
}

void FreeTypeHandleOperator::setCurrentHandleType(FreeTypeSelectedItem::HandleType handleType)
{
    m_handleType = handleType;
}

void FreeTypeHandleOperator::setShiftPressed(bool isVisible)
{
    m_isShiftPressed = isVisible;
}

void FreeTypeHandleOperator::disposePressEvent(QMouseEvent* event)
{
    m_pos = event->pos();
    m_scenePos = m_pRenderWidget->mapToScene(m_pos);

    auto pItem = m_pRenderWidget->getScaledItemHandleItem();
    if (pItem)
    {
        QRectF nRect = pItem->getSelectedRect();
        m_nSelectRectWidth = nRect.width();
        m_nSelectRectHeight = nRect.height();

        // Scale Value
        m_scaleValueMaps.clear();

        // Fill Factor Maps
        m_fixedPosFactorMaps.clear();
        auto selectedItems = m_pRenderWidget->scene()->selectedItems();
        for (auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
        {
            FreeTypeGlyphItem* pGlyphItem = dynamic_cast<FreeTypeGlyphItem*>(*iter);
            if (!pGlyphItem)
                continue;

            // Insert Factor Maps
            QVector2D factor;
            QRectF tempRect1 = pGlyphItem->boundingRect();
            QRectF tempRect2 = pItem->getSelectedRect();
            tempRect1 = QRectF(pGlyphItem->pos() + tempRect1.topLeft(), \
                               pGlyphItem->pos() + tempRect1.bottomRight());

            qreal factorX = (tempRect1.left() - tempRect2.left()) * 1.0 / tempRect2.width();
            qreal factorY = (tempRect1.top() - tempRect2.top()) * 1.0 / tempRect2.height();
            factor.setX(factorX);
            factor.setY(factorY);
            m_fixedPosFactorMaps.insert(pGlyphItem, factor);
        }
    }

    // Set Cussor Status
    processMouseCursor(m_pRenderWidget, (int)m_handleType);
}

void FreeTypeHandleOperator::disposeMoveEvent(QMouseEvent* event)
{
    QPoint currentPos = event->pos();
    QPointF scenePos = m_pRenderWidget->mapToScene(currentPos);

    qreal nInterValWidth = scenePos.x() - m_scenePos.x();
    qreal nInterValHeight = scenePos.y() - m_scenePos.y();

    auto pItem = m_pRenderWidget->getScaledItemHandleItem();
    if (pItem)
    {
        QRectF nRect = pItem->getSelectedRect();
        nRect = calcResultRect(nRect, nInterValWidth, nInterValHeight);

        pItem->setSelectedRect(nRect);

        m_scenePos = scenePos;

        scaleTranslateGlyphItems(nRect.width() * 1.0 / m_nSelectRectWidth, \
                                 nRect.height() * 1.0 / m_nSelectRectHeight);

//        m_nSelectRectWidth = nRect.width();
//        m_nSelectRectHeight = nRect.height();
    }

    // Set Cussor Status
    processMouseCursor(m_pRenderWidget, (int)m_handleType);
}

void FreeTypeHandleOperator::disposeReleaseEvent(QMouseEvent* event)
{
    m_pRenderWidget->viewport()->setCursor(Qt::ArrowCursor);
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

QRectF FreeTypeHandleOperator::calcResultRect(const QRectF& rect, qreal xInterval, qreal yInterval)
{
    QRectF result;

    switch (m_handleType) {
    case FreeTypeSelectedItem::t_TopLeftHandle:
        result = QRectF(QPointF(rect.topLeft().x() + xInterval, rect.topLeft().y() + yInterval), \
                        rect.bottomRight());
        break;

    case FreeTypeSelectedItem::t_TopMiddleHandle:
        result = QRectF(QPointF(rect.left(), rect.top() + yInterval), rect.bottomRight());
        break;

    case FreeTypeSelectedItem::t_TopRightHandle:
        result = QRectF(QPointF(rect.left(), rect.top() + yInterval), \
                        QPointF(rect.right() + xInterval, rect.bottom()));
        break;

    case FreeTypeSelectedItem::t_LeftMiddleHandle:
        result = QRectF(QPointF(rect.left() + xInterval, rect.top()), rect.bottomRight());
        break;

    case FreeTypeSelectedItem::t_RightMiddleHandle:
        result = QRectF(rect.topLeft(), QPointF(rect.right() + xInterval, rect.bottom()));
        break;

    case FreeTypeSelectedItem::t_BottomLeftHandle:
        result = QRectF(QPointF(rect.left() + xInterval, rect.top()), \
                        QPointF(rect.right(), rect.bottom() + yInterval));
        break;

    case FreeTypeSelectedItem::t_BottomMiddleHandle:
        result = QRectF(rect.topLeft(), QPointF(rect.right(), rect.bottom() + yInterval));
        break;

    case FreeTypeSelectedItem::t_BottomRightHandle:
        result = QRectF(rect.topLeft(), QPointF(rect.right() + xInterval, rect.bottom() + yInterval));
        break;

    default:
        break;
    }

    return result;
}

void FreeTypeHandleOperator::scaleTranslateGlyphItems(qreal xScaleValue, qreal yScaleValue)
{
    auto selectedItem = m_pRenderWidget->scene()->selectedItems();
    auto pSelectRectItem = m_pRenderWidget->getScaledItemHandleItem();

    for (auto iter = selectedItem.begin(); iter != selectedItem.end(); ++iter)
    {
        FreeTypeGlyphItem* pSelectedItem = dynamic_cast<FreeTypeGlyphItem*>(*iter);
        if (pSelectedItem == nullptr)
            continue;

        // Find Maps
        if (m_scaleValueMaps.find(pSelectedItem) == m_scaleValueMaps.end())
        {
            qreal xSrcScaleValue, ySrcScaleValue;
            pSelectedItem->getScaleValue(xSrcScaleValue, ySrcScaleValue);
            m_scaleValueMaps.insert(pSelectedItem, QVector2D(xSrcScaleValue, ySrcScaleValue));
        }

        // Set Fix Pos
        if (m_fixedPosFactorMaps.find(pSelectedItem) != m_fixedPosFactorMaps.end())
        {
            QVector2D factor = m_fixedPosFactorMaps[pSelectedItem];
            qreal xPt = factor.x() * pSelectRectItem->getSelectedRect().width() + pSelectRectItem->getSelectedRect().left();
            qreal yPt = factor.y() * pSelectRectItem->getSelectedRect().height() + pSelectRectItem->getSelectedRect().top();

            pSelectedItem->setScaleFixedPos(QPointF(xPt, yPt));
        }

        // Get Real Scale Value
        qreal xSrcScaleValue = m_scaleValueMaps[pSelectedItem].x();
        qreal ySrcScaleValue = m_scaleValueMaps[pSelectedItem].y();
        pSelectedItem->setScaleValue(xScaleValue * xSrcScaleValue, yScaleValue * ySrcScaleValue);
    }
}

void FreeTypeHandleOperator::processMouseCursor(FreeTypeRenderWidget* pRenderWidget, int type)
{
    FreeTypeSelectedItem::HandleType handleType = (FreeTypeSelectedItem::HandleType)type;

    switch (handleType) {
    case FreeTypeSelectedItem::t_TopLeftHandle:
    case FreeTypeSelectedItem::t_BottomRightHandle:
        pRenderWidget->viewport()->setCursor(Qt::SizeFDiagCursor);
        return;

    case FreeTypeSelectedItem::t_TopMiddleHandle:
    case FreeTypeSelectedItem::t_BottomMiddleHandle:
        pRenderWidget->viewport()->setCursor(Qt::SizeVerCursor);
        return;

    case FreeTypeSelectedItem::t_TopRightHandle:
    case FreeTypeSelectedItem::t_BottomLeftHandle:
        pRenderWidget->viewport()->setCursor(Qt::SizeBDiagCursor);
        return;

    case FreeTypeSelectedItem::t_LeftMiddleHandle:
    case FreeTypeSelectedItem::t_RightMiddleHandle:
        pRenderWidget->viewport()->setCursor(Qt::SizeHorCursor);
        return;

    default:
        break;
    }

    return pRenderWidget->viewport()->setCursor(Qt::ArrowCursor);
}

// ---------------------------------------------------------------------
// Point Select Operator
FreeTypePointHandleOperator::FreeTypePointHandleOperator(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypePointHandleOperator::~FreeTypePointHandleOperator()
{

}

void FreeTypePointHandleOperator::disposePressEvent(QMouseEvent* event)
{
    if (m_pGlyphItem == nullptr || m_nCurrentSelectedIndex < 0)
        return;

    if (m_operatorPointType == 0 || m_operatorPointType == 2)
        m_pGlyphItem->setCurrentHandleIndex(m_nCurrentSelectedIndex);
}

void FreeTypePointHandleOperator::disposeMoveEvent(QMouseEvent* event)
{
    if (m_pGlyphItem == nullptr || m_nCurrentSelectedIndex < 0)
        return;

    QPointF scenePos = m_pRenderWidget->mapToScene(event->pos());
    QPointF itemPos = m_pGlyphItem->mapFromScene(scenePos);

    m_pGlyphItem->changedPoint(m_nCurrentSelectedIndex, itemPos);
}

void FreeTypePointHandleOperator::disposeReleaseEvent(QMouseEvent* event)
{
    m_operatorPointType = 0;
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

void FreeTypePointHandleOperator::initCurrentInfo(int index, FreeTypeGlyphItem* pItem, int pointType)
{
    m_nCurrentSelectedIndex = index;
    m_pGlyphItem = pItem;
    m_operatorPointType = pointType;
}

bool FreeTypePointHandleOperator::needDisposeThisOper(FreeTypeRenderWidget* pRenderWidget, QMouseEvent* event, \
                                                      FreeTypeGlyphItem*& pItem, int& index, int& pointType)
{
    QPointF scenePos = pRenderWidget->mapToScene(event->pos());
    auto selectedItems = pRenderWidget->scene()->selectedItems();
    qreal r = 6;

    for (auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
    {
        FreeTypeGlyphItem* pGlyphItem = dynamic_cast<FreeTypeGlyphItem*>(*iter);
        if (pGlyphItem == nullptr)
            continue;

        FreeTypeCore::PointInfos pointInfos;
        pGlyphItem->getCurrentPointInfo(pointInfos);

        // Adjust Normal Point
        QPointF itemPos = pGlyphItem->mapFromScene(scenePos);
        for (int i=0; i<pointInfos.size(); ++i)
        {
            if ((pointInfos[i].pointType == 0 || pointInfos[i].pointType == 2) && \
                g_FreeTypeTool->getDistance(pointInfos[i].pos, itemPos) < r)
            {
                pItem = pGlyphItem;
                index = i;
                pointType = pointInfos[i].pointType;
                return true;
            }
        }

        int tempIndex = -1;
        QPointF pos;
        // Adjust Handle Point - left
        bool result = pGlyphItem->getSelectLeftHandlePoint(pos, tempIndex);
        if (result && g_FreeTypeTool->getDistance(pos, itemPos) < r)
        {
            pItem = pGlyphItem;
            index = tempIndex;
            pointType = pointInfos[index].pointType;

            return true;
        }
        // Adjust Handle Point - right
        result = pGlyphItem->getSelectRightHandlePoint(pos, tempIndex);
        if (result && g_FreeTypeTool->getDistance(pos, itemPos) < r)
        {
            pItem = pGlyphItem;
            index = tempIndex;
            pointType = pointInfos[index].pointType;

            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------
// Point Delete Operator
FreeTypePointHandleDeleteOperator::FreeTypePointHandleDeleteOperator(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypePointHandleDeleteOperator::~FreeTypePointHandleDeleteOperator()
{

}

void FreeTypePointHandleDeleteOperator::initCurrentInfo(int index, FreeTypeGlyphItem* pItem)
{
    m_nCurrentSelectedIndex = index;
    m_pGlyphItem = pItem;
}

void FreeTypePointHandleDeleteOperator::disposePressEvent(QMouseEvent* event)
{
    if (m_pGlyphItem == nullptr || m_nCurrentSelectedIndex < 0)
        return;

    m_pGlyphItem->removePoint(m_nCurrentSelectedIndex);
}

void FreeTypePointHandleDeleteOperator::disposeMoveEvent(QMouseEvent* event)
{

}

void FreeTypePointHandleDeleteOperator::disposeReleaseEvent(QMouseEvent* event)
{
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

// ---------------------------------------------------------------------
// Point Spit Operator
FreeTypePointHandleSpitOperator::FreeTypePointHandleSpitOperator(FreeTypeRenderWidget* pRenderWidget)
    :FreeTypeOperatorBase(pRenderWidget)
{

}

FreeTypePointHandleSpitOperator::~FreeTypePointHandleSpitOperator()
{

}

void FreeTypePointHandleSpitOperator::disposePressEvent(QMouseEvent* event)
{
    if (m_pGlyphItem == nullptr || m_nCurrentSelectedIndex < 0)
        return;

    m_pGlyphItem->setCurrentHandleIndex(m_nCurrentSelectedIndex);
    m_pGlyphItem->addCopyPoint(m_nCurrentSelectedIndex);
}

void FreeTypePointHandleSpitOperator::disposeMoveEvent(QMouseEvent* event)
{

}

void FreeTypePointHandleSpitOperator::disposeReleaseEvent(QMouseEvent* event)
{
    FreeTypeDefOper* pDefOper = new FreeTypeDefOper(m_pRenderWidget);
    m_pRenderWidget->setOperator(pDefOper);
}

void FreeTypePointHandleSpitOperator::initCurrentInfo(int index, FreeTypeGlyphItem* pItem)
{
    m_nCurrentSelectedIndex = index;
    m_pGlyphItem = pItem;
}

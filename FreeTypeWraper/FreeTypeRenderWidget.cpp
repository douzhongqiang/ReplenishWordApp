#include "FreeTypeRenderWidget.h"
#include <QDebug>
#include <QPainter>
#include "FreeTypeOperator.h"
#include "FreeTypeGlyphItem.h"
#include "FreeTypeSelectedItem.h"
#include "FreeTypeConfig.h"

FreeTypeRenderWidget::FreeTypeRenderWidget(QWidget* parent)
    : QGraphicsView(parent)
    , m_cSelectedPenColor(200, 100, 100)
    , m_cSelectedBrushColor(0, 0, 200, 100)
    , m_currentOperatorType(t_MoveOperator)
{
    m_pOperator = new FreeTypeDefOper(this);
    this->setMouseTracking(true);

    m_pScene = new QGraphicsScene(this);
    this->setScene(m_pScene);
    this->setSceneRect(QRect(0, 0, 1200, 1200));

    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    initSceneItems();
    QObject::connect(m_pScene, &QGraphicsScene::selectionChanged, \
                     this, &FreeTypeRenderWidget::onItemSelectionChanged);
    QObject::connect(g_FreeTypeConfig, &FreeTypeConfig::handleEnabledChanged, \
                     this, &FreeTypeRenderWidget::onItemSelectionChanged);
}

FreeTypeRenderWidget::~FreeTypeRenderWidget()
{

}

void FreeTypeRenderWidget::setOperator(FreeTypeOperatorBase* pOperator)
{
    delete m_pOperator;
    m_pOperator = pOperator;
}

void FreeTypeRenderWidget::setFreeTypeCore(QSharedPointer<FreeTypeCore> freeTypeCore)
{
    m_pFreeTypeCore = freeTypeCore;
    this->update();
}

void FreeTypeRenderWidget::loadCurrentFont(const QString& fontPath)
{
    if (m_pFreeTypeCore.isNull())
        m_pFreeTypeCore = QSharedPointer<FreeTypeCore>(new FreeTypeCore);

    m_pFreeTypeCore->init(fontPath);
    this->update();
}

void FreeTypeRenderWidget::setCurrentRender(const QString& renderString)
{
    if (m_pFreeTypeCore.isNull())
        return;

    ushort unicode = renderString.unicode()->unicode();
    m_pFreeTypeCore->loadChar(unicode, renderString);

    QVector<FreeTypeCore::PointInfos> pointInfos;
    m_pFreeTypeCore->getCurrentPointInfos(pointInfos);
    for (auto iter = pointInfos.begin(); iter != pointInfos.end(); ++iter)
    {
        FreeTypeGlyphItem* pGlyphItem = new FreeTypeGlyphItem;
        pGlyphItem->setCurrentPointInfo(*iter);
        m_pScene->addItem(pGlyphItem);
        pGlyphItem->setPos(50, 100);

        QObject::connect(pGlyphItem, &FreeTypeGlyphItem::signalItemPosChanged, \
                         this, &FreeTypeRenderWidget::onItemSelectionChanged);
    }
}

void FreeTypeRenderWidget::mousePressEvent(QMouseEvent* event)
{
    m_pOperator->disposePressEvent(event);
    return QWidget::mousePressEvent(event);
}

void FreeTypeRenderWidget::mouseReleaseEvent(QMouseEvent* event)
{
    m_pOperator->disposeReleaseEvent(event);
    return QWidget::mouseReleaseEvent(event);
}

void FreeTypeRenderWidget::mouseMoveEvent(QMouseEvent *event)
{
    m_pOperator->disposeMoveEvent(event);
    return QWidget::mouseMoveEvent(event);
}

void FreeTypeRenderWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete)
    {
        auto items = this->scene()->selectedItems();
        for (auto iter = items.begin(); iter != items.end(); ++iter)
            delete (*iter);
    }

    return QWidget::keyPressEvent(event);
}

// Set Select Rect About
void FreeTypeRenderWidget::setSelectRectEnabled(bool isEnabled)
{
    m_isSelectEnabled = isEnabled;
    this->viewport()->update();
}

void FreeTypeRenderWidget::setSelectRect(const QRect& rect)
{
    QPointF startPos = this->mapToScene(rect.topLeft());
    QPointF endPos = this->mapToScene(rect.bottomRight());
    m_selectRect = QRect(QPoint(startPos.x(), startPos.y()), \
                           QPoint(endPos.x(), endPos.y()));

    this->viewport()->update();
}

void FreeTypeRenderWidget::paintSelectRect(QPainter* painter)
{
    if (!m_isSelectEnabled)
        return;

    painter->save();

    painter->setPen(QColor(0, 0, 200));
    painter->setBrush(QColor(0, 0, 255, 120));
    painter->drawRect(m_selectRect);

    painter->restore();
}

void FreeTypeRenderWidget::drawBackground(QPainter * painter, const QRectF & rect)
{
    return QGraphicsView::drawBackground(painter, rect);
}

void FreeTypeRenderWidget::drawForeground(QPainter * painter, const QRectF & rect)
{
    if (!m_isSelectEnabled)
        return QGraphicsView::drawForeground(painter, rect);

    QPen pen;
    pen.setWidth(1);
    pen.setColor(m_cSelectedPenColor);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);

    painter->setBrush(m_cSelectedBrushColor);
    painter->drawRect(m_selectRect);

    return QGraphicsView::drawForeground(painter, rect);
}

// Set / Get Current Operator Tool
void FreeTypeRenderWidget::setCurrentOperatorTool(OperatorType type)
{
    m_currentOperatorType = type;
}

FreeTypeRenderWidget::OperatorType FreeTypeRenderWidget::getCurrentOperatorTool(void)
{
    return m_currentOperatorType;
}

void FreeTypeRenderWidget::initSceneItems(void)
{
    m_pSelectItem = new FreeTypeSelectedItem;
    m_pScene->addItem(m_pSelectItem);

    m_pSelectItem->setZValue(10);
    m_pSelectItem->setVisible(false);
}

void FreeTypeRenderWidget::onItemSelectionChanged(void)
{
    auto selectedItems = m_pScene->selectedItems();
    if (selectedItems.size() <= 0 || g_FreeTypeConfig->isHandleEnabled())
    {
        m_pSelectItem->setVisible(false);
        return;
    }

    QPainterPath path;
    for (auto iter = selectedItems.begin(); iter != selectedItems.end(); ++iter)
    {
        QRectF rectF = (*iter)->boundingRect();
        QPointF pos = (*iter)->pos();

        rectF = QRectF(rectF.x() + pos.x(), rectF.y() + pos.y(), rectF.width(), rectF.height());
        path.addRect(rectF);
    }

    m_pSelectItem->setSelectedRect(path.boundingRect());
    m_pSelectItem->setVisible(true);
}

FreeTypeSelectedItem* FreeTypeRenderWidget::getScaledItemHandleItem(void)
{
    return m_pSelectItem;
}


// Cursor About
void FreeTypeRenderWidget::setPenHandleCursor(const QCursor& cursor)
{
    m_penHandleCursor = cursor;
}

QCursor FreeTypeRenderWidget::getPenHandleCursor(void)
{
    return m_penHandleCursor;
}

void FreeTypeRenderWidget::setRotateHandleCursor(const QCursor& cursor)
{
    m_rotateHandleCursor = cursor;
}

QCursor FreeTypeRenderWidget::getRotateHandleCursor(void)
{
    return m_rotateHandleCursor;
}

void FreeTypeRenderWidget::clearSelectedItem(void)
{
    auto items = m_pScene->selectedItems();
    for (auto iter = items.begin(); iter != items.end(); ++iter)
        delete *iter;
}

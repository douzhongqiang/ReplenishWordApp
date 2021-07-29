#include "FreeTypeRenderWidget.h"
#include <QDebug>
#include <QPainter>
#include "FreeTypeOperator.h"
#include "FreeTypeGlyphItem.h"

FreeTypeRenderWidget::FreeTypeRenderWidget(QWidget* parent)
    : QGraphicsView(parent)
    , m_cSelectedPenColor(200, 100, 100)
    , m_cSelectedBrushColor(0, 0, 200, 100)
{
    m_pOperator = new FreeTypeDefOper(this);
    this->setMouseTracking(true);

    m_pScene = new QGraphicsScene(this);
    this->setScene(m_pScene);
    this->setSceneRect(QRect(0, 0, 1200, 1200));

    this->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
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

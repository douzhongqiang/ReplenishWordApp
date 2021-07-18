#include "FreeTypeRenderWidget.h"
#include <QDebug>
#include <QPainter>

FreeTypeRenderWidget::FreeTypeRenderWidget(QWidget* parent)
    :QWidget(parent)
{

}

FreeTypeRenderWidget::~FreeTypeRenderWidget()
{

}

void FreeTypeRenderWidget::setFreeTypeCore(QSharedPointer<FreeTypeCore> freeTypeCore)
{
    m_pFreeTypeCore = freeTypeCore;
}

void FreeTypeRenderWidget::loadCurrentFont(const QString& fontPath)
{
    if (m_pFreeTypeCore.isNull())
        m_pFreeTypeCore = QSharedPointer<FreeTypeCore>(new FreeTypeCore);

    m_pFreeTypeCore->init(fontPath);
}

void FreeTypeRenderWidget::setCurrentRender(const QString& renderString)
{
    if (m_pFreeTypeCore.isNull())
        return;

    ushort unicode = renderString.unicode()->unicode();
    m_pFreeTypeCore->loadChar(unicode);
}

void FreeTypeRenderWidget::paintEvent(QPaintEvent* event)
{
    QPainter pinter(this);

    if (m_pFreeTypeCore)
        m_pFreeTypeCore->render(&pinter);

    return QWidget::paintEvent(event);
}

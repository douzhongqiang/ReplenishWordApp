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
    this->update();
}

void FreeTypeRenderWidget::paintEvent(QPaintEvent* event)
{
    QPainter pinter(this);

    pinter.translate(this->width() / 2, this->height() / 2);
    pinter.scale(1.0f, -1.0f);

    if (m_pFreeTypeCore)
        m_pFreeTypeCore->render(&pinter);

    return QWidget::paintEvent(event);
}

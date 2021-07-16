#include "FreeTypeRenderWidget.h"
#include <QDebug>

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

    qDebug() << renderString;
//    m_pFreeTypeCore->loadChar();
}

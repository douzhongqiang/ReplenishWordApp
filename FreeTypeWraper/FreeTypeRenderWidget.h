#ifndef FREETYPERENDERWIDGET_H
#define FREETYPERENDERWIDGET_H

#include <QWidget>
#include <QSharedPointer>
#include "FreeTypeCore.h"
#include "FreeTypeWraper_global.h"

class FREETYPEWRAPER_EXPORT FreeTypeRenderWidget : public QWidget
{
    Q_OBJECT

public:
    FreeTypeRenderWidget(QWidget* parent = nullptr);
    ~FreeTypeRenderWidget();

    void loadCurrentFont(const QString& fontPath);
    void setCurrentRender(const QString& renderString);

    void setFreeTypeCore(QSharedPointer<FreeTypeCore> freeTypeCore);

private:
    QSharedPointer<FreeTypeCore> m_pFreeTypeCore;

    void paintEvent(QPaintEvent* event);
};

#endif

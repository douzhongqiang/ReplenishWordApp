#include "FreeTypeTool.h"
#include <QApplication>
#include <QStyle>

FreeTypeTool::FreeTypeTool(QObject* parent)
    :QObject(parent)
{

}

FreeTypeTool::~FreeTypeTool()
{

}

FreeTypeTool* FreeTypeTool::getInstance(void)
{
    static FreeTypeTool instance;
    return &instance;
}

qreal FreeTypeTool::getDistance(const QPointF p1, const QPointF p2)
{
    return sqrt((p1.x() - p2.x()) * (p1.x() - p2.x()) + (p1.y() - p2.y()) * (p1.y() - p2.y()));
}

// apply current style
void FreeTypeTool::applyCurrentStyle(QWidget* pWidget, const QString& propertyValue, bool needPolish)
{
    pWidget->setProperty(TOSTRING(CustomStyle), propertyValue);

    if (needPolish)
    {
        qApp->style()->unpolish(pWidget);
        qApp->style()->polish(pWidget);
    }
}

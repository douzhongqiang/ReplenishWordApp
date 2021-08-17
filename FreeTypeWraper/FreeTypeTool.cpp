#include "FreeTypeTool.h"

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

#ifndef FREETYPETOOL_H
#define FREETYPETOOL_H

#include <QObject>
#include "FreeTypeWraper_global.h"

#define g_FreeTypeTool FreeTypeTool::getInstance()

class FREETYPEWRAPER_EXPORT FreeTypeTool : public QObject
{
    Q_OBJECT

public:
    static FreeTypeTool* getInstance(void);

    qreal getDistance(const QPointF p1, const QPointF p2);

private:
    FreeTypeTool(QObject* parent = nullptr);
    ~FreeTypeTool();
};

#endif

#ifndef FREETYPETOOL_H
#define FREETYPETOOL_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QVariant>
#include "FreeTypeWraper_global.h"

#define g_FreeTypeTool FreeTypeTool::getInstance()

#ifndef TOSTRING
#define TOSTRING(x) #x
#endif

class FREETYPEWRAPER_EXPORT FreeTypeTool : public QObject
{
    Q_OBJECT

public:
    static FreeTypeTool* getInstance(void);

    qreal getDistance(const QPointF p1, const QPointF p2);

    // apply current style
    void applyCurrentStyle(QWidget* pWidget, const QString& propertyValue, bool needPolish = false);

private:
    FreeTypeTool(QObject* parent = nullptr);
    ~FreeTypeTool();
};

#endif

#ifndef FREETYPECONFIG_H
#define FREETYPECONFIG_H

#include <QObject>
#include "FreeTypeWraper_global.h"

#define g_FreeTypeConfig FreeTypeConfig::getInstance()

class FREETYPEWRAPER_EXPORT FreeTypeConfig : public QObject
{
    Q_OBJECT

public:
    static FreeTypeConfig* getInstance(void);

    // set/get handle enabled
    void setHandleEnabled(bool isEnabled);
    bool isHandleEnabled(void);

private:
    FreeTypeConfig(QObject* parent = nullptr);
    ~FreeTypeConfig();

    bool m_isHandleEnabled = false;

signals:
    void handleEnabledChanged(bool isEnabled);
};

#endif

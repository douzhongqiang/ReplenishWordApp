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

    // set/get handle delete Mode
    void setHandlePointDeleteMode(bool isEnabled);
    bool isHandlePointDeleteMode(void);

    // set/get handle spit point enabled
    void setHandlePointSpitEnabled(bool isEnabled);
    bool isHandlePointSpitEnabled(void);

    // set/get handle Adsorb Point Enabled
    void setHandlePointAdsorbEnabled(bool isEnabeld);
    bool isHandlePointAdsorbEnabled(void);

private:
    FreeTypeConfig(QObject* parent = nullptr);
    ~FreeTypeConfig();

    bool m_isHandleEnabled = false;
    bool m_isHandlePointDeleteMode = false;
    bool m_isHandleSpitPointEnabled = false;
    bool m_isHandleAdsorbPointEnabled = false;

signals:
    void handleEnabledChanged(bool isEnabled);
    void handlePointDeleteModeChanged(bool isEnabled);
    void handlePointSpitEnabledChanged(bool isEnabled);
    void handlePointAdsorbEnabledChanged(bool isEnabled);
};

#endif

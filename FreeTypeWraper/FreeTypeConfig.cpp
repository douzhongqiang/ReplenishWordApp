#include "FreeTypeConfig.h"

FreeTypeConfig::FreeTypeConfig(QObject* parent)
    :QObject(parent)
{

}

FreeTypeConfig::~FreeTypeConfig()
{

}

FreeTypeConfig* FreeTypeConfig::getInstance(void)
{
    static FreeTypeConfig instance;
    return &instance;
}

// set/get handle enabled
void FreeTypeConfig::setHandleEnabled(bool isEnabled)
{
    if (m_isHandleEnabled != isEnabled)
    {
        m_isHandleEnabled = isEnabled;
        emit handleEnabledChanged(m_isHandleEnabled);
    }
}

bool FreeTypeConfig::isHandleEnabled(void)
{
    return m_isHandleEnabled;
}

// set/get handle delete Mode
void FreeTypeConfig::setHandlePointDeleteMode(bool isEnabled)
{
    if (m_isHandlePointDeleteMode != isEnabled)
    {
        m_isHandlePointDeleteMode = isEnabled;
        emit handlePointDeleteModeChanged(m_isHandlePointDeleteMode);
    }
}

bool FreeTypeConfig::isHandlePointDeleteMode(void)
{
    return m_isHandlePointDeleteMode;
}

// set/get handle spit point enabled
void FreeTypeConfig::setHandlePointSpitEnabled(bool isEnabled)
{
    if (m_isHandleSpitPointEnabled != isEnabled)
    {
        m_isHandleSpitPointEnabled = isEnabled;
        emit handlePointSpitEnabledChanged(m_isHandleSpitPointEnabled);
    }
}

bool FreeTypeConfig::isHandlePointSpitEnabled(void)
{
    return m_isHandleSpitPointEnabled;
}

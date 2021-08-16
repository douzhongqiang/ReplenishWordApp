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

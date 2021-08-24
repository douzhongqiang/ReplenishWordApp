#ifndef HPNEARCHARACTORCORE_H
#define HPNEARCHARACTORCORE_H

#include <QObject>
#include "HPNearCharacter_global.h"

class HPNEARCHARACTER_EXPORT HPNearCharactorCore : public QObject
{
    Q_OBJECT

public:
    HPNearCharactorCore(QObject* parent = nullptr);
    ~HPNearCharactorCore();
};

#endif

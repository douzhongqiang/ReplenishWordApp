#ifndef FREETYPECORE_H
#define FREETYPECORE_H

#include <QObject>

class FreeTypeCore : public QObject
{
    Q_OBJECT

public:
    FreeTypeCore(QObject* parent = nullptr);
    ~FreeTypeCore();

    void init(void);
};

#endif

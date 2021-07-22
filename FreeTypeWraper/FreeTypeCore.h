#ifndef FREETYPECORE_H
#define FREETYPECORE_H

#include <QObject>
#include <QPointF>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <QPainter>
#include "FreeTypeWraper_global.h"

class FREETYPEWRAPER_EXPORT FreeTypeCore : public QObject
{
    Q_OBJECT

public:
    struct PointInfo
    {
        QPointF pos;
        int pointType = 0;      // 0 : Normal. 1: quad Control, 2: cubic Control
    };

    typedef QVector<PointInfo> PointInfos;

public:
    FreeTypeCore(QObject* parent = nullptr);
    ~FreeTypeCore();

    bool init(const QString& fontPath);
    void relsase(void);

    void setRenderPixelSize(int size);

    bool loadChar(unsigned int unicode);

    void render(QPainter* painter);

private:
    FT_Face m_pFace = nullptr;
    FT_Library m_pFreetypeLibrary = nullptr;

    int m_nFacePixelSize = 100;

    void loadCurrentFace(void);
    QVector<PointInfos> m_PointInfos;

private:
    static int moveTo(const FT_Vector* to, void* user);
    static int lineTo(const FT_Vector* to, void* user);
    static int conicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int cubicto(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
};

#endif

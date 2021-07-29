#ifndef FREETYPECORE_H
#define FREETYPECORE_H

#include <QObject>
#include <QPointF>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <QPainter>
#include <QPainterPath>
#include "FreeTypeWraper_global.h"

class FREETYPEWRAPER_EXPORT FreeTypeCore : public QObject
{
    Q_OBJECT

public:
    struct PointInfo
    {
        QPointF pos;
        int pointType = 0;      // 0 : Normal. 1: Control
        int srcTagType = 0;
    };

    typedef QVector<PointInfo> PointInfos;

public:
    FreeTypeCore(QObject* parent = nullptr);
    ~FreeTypeCore();

    bool init(const QString& fontPath);
    void relsase(void);

    void setRenderPixelSize(int size);

    // Load Unicode Char
    bool loadChar(unsigned int unicode, QString str = "");

    // Get Loaded Point Infos
    void getCurrentPointInfos(QVector<PointInfos>& pointInfos);

    void render(QPainter* painter);
    void biquadTo(QPainterPath& path, QPointF start, QPointF c1, QPointF c2, QPointF c3, QPointF to);
    void bezierCurve(QPainterPath& path, const QVector<QPointF>& vec);

private:
    FT_Face m_pFace = nullptr;
    FT_Library m_pFreetypeLibrary = nullptr;

    int m_nFacePixelSize = 100;

    void loadCurrentFace(void);
    QVector<PointInfos> m_PointInfos;
    PointInfos m_currentPointInfo;

    QPainterPath m_path;

    int m_nGlyphWidth = 0;
    int m_nGlyphHeight = 0;
    QString m_renderString;

    void drawContour(int index, QPainterPath& tempPath, QPainter* painter);
    void drawHandle(QPainter* painter);

    float factorial(int number);

private:
    static int moveTo(const FT_Vector* to, void* user);
    static int lineTo(const FT_Vector* to, void* user);
    static int conicTo(const FT_Vector* control, const FT_Vector* to, void* user);
    static int cubicto(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user);
};

#endif

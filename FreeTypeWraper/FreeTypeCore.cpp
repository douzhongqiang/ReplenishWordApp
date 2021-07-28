#include "FreeTypeCore.h"
#include <QDebug>
#include <freetype/ftimage.h>
#include <freetype/ftoutln.h>

FreeTypeCore::FreeTypeCore(QObject* parent)
    :QObject(parent)
{

}

FreeTypeCore::~FreeTypeCore()
{

}

bool FreeTypeCore::init(const QString& fontPath)
{
    if (FT_Init_FreeType(&m_pFreetypeLibrary))
    {
        qDebug() << "Init FreeType Library Error!";
        return false;
    }

    // Load Font
    if (FT_New_Face(m_pFreetypeLibrary, fontPath.toStdString().c_str(), 0, &m_pFace))
    {
        qDebug() << "Load Font Error!";
        return false;
    }

    // Set PixSize
    FT_Set_Pixel_Sizes(m_pFace, 0, m_nFacePixelSize);

    return true;
}

void FreeTypeCore::relsase(void)
{
    FT_Done_Face(m_pFace);
    FT_Done_FreeType(m_pFreetypeLibrary);
}

void FreeTypeCore::setRenderPixelSize(int size)
{
    m_nFacePixelSize = size;
    FT_Set_Pixel_Sizes(m_pFace, 0, m_nFacePixelSize);
}

bool FreeTypeCore::loadChar(unsigned int unicode)
{
    int glyph_index = FT_Get_Char_Index(m_pFace, unicode);
    if (glyph_index <= 0)
    {
        qDebug() << "Get Char Index Error!";
        return false;
    }

    if (FT_Load_Glyph(m_pFace, glyph_index, 0))
    {
        qDebug() << "Load Glyph Error!";
        return false;
    }

    loadCurrentFace();


    m_path.clear();
    FT_Outline_Funcs callback;
    callback.move_to = FreeTypeCore::moveTo;
    callback.line_to = FreeTypeCore::lineTo;
    callback.conic_to = FreeTypeCore::conicTo;
    callback.cubic_to = FreeTypeCore::cubicto;
    callback.shift = 0;
    callback.delta = 0;

    FT_Error error = FT_Outline_Decompose(&m_pFace->glyph->outline, &callback, this);
    if (error)
        qDebug() << "FT_Outline_Decompose Called Error!";

    return true;
}

void FreeTypeCore::loadCurrentFace(void)
{
    FT_Outline outline = m_pFace->glyph->outline;

    m_PointInfos.clear();
    int nContours = outline.n_contours;
    int index = 0;

    PointInfos tempInfos;
    for (int i=0; i<outline.n_points; ++i)
    {
        PointInfo posInfo;
        QPoint pos(outline.points[i].x / 64.0, outline.points[i].y / 64.0);
        posInfo.pos = pos;
        if (outline.tags[i] & 0x01)
            posInfo.pointType = 0;
        else
            posInfo.pointType = 1;
        posInfo.srcTagType = outline.tags[i];

        tempInfos.push_back(posInfo);

        if (outline.contours[index] == i)
        {
            m_PointInfos.push_back(tempInfos);
            tempInfos.clear();

            index++;
        }
    }

    int nIndex = 0;
    for (auto iter = m_PointInfos.begin(); iter != m_PointInfos.end(); ++iter)
    {
        qDebug() << ++nIndex << "====================================";

        for (auto iter2 = iter->begin(); iter2 != iter->end(); ++iter2)
        {
            qDebug() << iter2->pos << iter2->pointType << iter2->srcTagType;
        }
    }
}

void FreeTypeCore::drawContour(int index, QPainter* painter)
{
    if (m_PointInfos.size() <= index)
        return;

    QPainterPath path;
    int nLastDrawedIndex = 0;
    QVector<PointInfo> pointInfos = m_PointInfos[index];
    for (int i=0; i<pointInfos.size(); ++i)
    {
        if (i == 0 && pointInfos[i].pointType == 0)
        {
            path.moveTo(pointInfos[i].pos);
            nLastDrawedIndex = 0;
            continue;
        }

        if (pointInfos[i].pointType == 0)
        {
            if (i - 1 == nLastDrawedIndex)
                path.lineTo(pointInfos[i].pos);
            else if (i - 2 == nLastDrawedIndex)
                path.quadTo(pointInfos[i - 1].pos, pointInfos[i].pos);
            else if (i - 3 == nLastDrawedIndex)
                path.cubicTo(pointInfos[i - 2].pos, pointInfos[i - 1].pos, pointInfos[i].pos);
//            else if (i - 4 == nLastDrawedIndex)
//                biquadTo(path, )
            else
            {
                while (nLastDrawedIndex != i)
                {
                    path.lineTo(pointInfos[++nLastDrawedIndex].pos);
                }
            }

            nLastDrawedIndex = i;
        }

        if (i == pointInfos.size() - 1)
        {
            if (pointInfos[i].pointType == 0)
                path.lineTo(pointInfos[0].pos);
            else
            {
                if (i - 1 == nLastDrawedIndex)
                    path.quadTo(pointInfos[i].pos, pointInfos[0].pos);
                else if (i - 2 == nLastDrawedIndex)
                    path.cubicTo(pointInfos[i - 1].pos, pointInfos[i].pos, pointInfos[0].pos);
            }
        }
    }

    painter->drawPath(path);
}

void FreeTypeCore::render(QPainter* painter)
{
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(120, 120, 120));
    painter->setPen(pen);
    painter->drawPath(m_path);

    painter->save();
    painter->translate(QPoint(0, 120));
    QVector<QColor> nColorVec;
    nColorVec << QColor(255, 0, 0) << QColor(0, 255, 0) << QColor(0, 0, 255) << \
                 QColor(255, 255, 0) << QColor(0, 255, 255) << QColor(255, 0, 255) << \
                 QColor(0, 0, 0) << QColor(128, 0, 128) << QColor(128, 128, 255);

    for (int i=0; i<m_PointInfos.size(); ++i)
    {
        QPen pen;
        pen.setWidth(1);
        //pen.setColor(nColorVec[i % nColorVec.size()]);
        painter->setPen(pen);

        drawContour(i, painter);
    }
    painter->restore();
}

void FreeTypeCore::biquadTo(QPainterPath& path, QPointF start, QPointF c1, QPointF c2, QPointF c3, QPointF to)
{

    for (int i=0; i<=100; ++i)
    {
        float t = i / 100;
        QPointF t1 = pow((1 - t), 4) * start;
        QPointF t2 = 4 * t * pow(1-t, 3) * c1;
        QPointF t3 = 6 * pow(t, 2) * pow(1-t, 2) * c2;
        QPointF t4 = 4 * pow(t, 3) * pow(1-t, 1) * c3;
        QPointF t5 = pow(t, 4) * to;

        QPointF destPos = t1 + t2 + t3 + t4 + t5;
        path.lineTo(destPos);
    }
}

int FreeTypeCore::moveTo(const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.moveTo(QPoint(to->x / 64.0f, to->y / 64.0f));
    qDebug() << __FUNCTION__ << to->x << ", " << to->y;
    return 0;
}

int FreeTypeCore::lineTo(const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.lineTo(QPoint(to->x / 64.0f, to->y / 64.0f));
    qDebug() << __FUNCTION__ << to->x << ", " << to->y;
    return 0;
}

int FreeTypeCore::conicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.quadTo(QPoint(control->x / 64.0f, control->y / 64.0f), QPoint(to->x / 64.0f, to->y / 64.0f));
    qDebug() << __FUNCTION__ << control->x << ", " << control->y << "; " << to->x << ", " << to->y;
    return 0;
}

int FreeTypeCore::cubicto(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.cubicTo(QPoint(control1->x / 64.0f, control1->y / 64.0f), \
                          QPoint(control2->x / 64.0f, control2->y / 64.0f), \
                          QPoint(to->x / 64.0f, to->y / 64.0f));

    qDebug() << __FUNCTION__ << control1->x << ", " << control1->y << "; " \
                             << control2->x << ", " << control2->y << "; " \
                             << to->x << ", " << to->y;
    return 0;
}

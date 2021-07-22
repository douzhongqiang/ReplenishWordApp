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


    FT_Outline_Funcs callback;
    callback.move_to = FreeTypeCore::moveTo;
    callback.line_to = FreeTypeCore::lineTo;
    callback.conic_to = FreeTypeCore::conicTo;
    callback.cubic_to = FreeTypeCore::cubicto;
    callback.shift = 0;
    callback.delta = 0;

    FT_Error error = FT_Outline_Decompose(&m_pFace->glyph->outline, &callback, nullptr);
    if (error)
        qDebug() << "FT_Outline_Decompose Called Error!";

    return true;
}

void FreeTypeCore::loadCurrentFace(void)
{
    FT_Outline outline = m_pFace->glyph->outline;

    int nContours = outline.n_contours;
    int index = 0;

    PointInfos tempInfos;
    for (int i=0; i<nContours; ++i)
    {
        if (outline.contours[index] == i)
        {
            m_PointInfos.push_back(tempInfos);
            tempInfos.clear();

            index++;
        }

        PointInfo posInfo;
        QPoint pos(outline.points[i].x, outline.points[i].y);
        posInfo.pos = pos;
        if (outline.tags[i] & 0x01)
            posInfo.pointType = 0;
        else if (outline.tags[i] & 0x02)
            posInfo.pointType = 1;

        tempInfos.push_back(posInfo);
    }
}

void FreeTypeCore::render(QPainter* painter)
{

}

int FreeTypeCore::moveTo(const FT_Vector* to, void* user)
{
    qDebug() << __FUNCTION__ << to->x << ", " << to->y;
    return 0;
}

int FreeTypeCore::lineTo(const FT_Vector* to, void* user)
{
    qDebug() << __FUNCTION__ << to->x << ", " << to->y;
    return 0;
}

int FreeTypeCore::conicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
    qDebug() << __FUNCTION__ << control->x << ", " << control->y << "; " << to->x << ", " << to->y;
    return 0;
}

int FreeTypeCore::cubicto(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
    qDebug() << __FUNCTION__ << control1->x << ", " << control1->y << "; " \
                             << control2->x << ", " << control2->y << "; " \
                             << to->x << ", " << to->y;
    return 0;
}

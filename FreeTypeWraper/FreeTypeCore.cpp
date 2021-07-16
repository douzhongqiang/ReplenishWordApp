#include "FreeTypeCore.h"
#include <QDebug>

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

    return true;
}

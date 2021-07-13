#include "FreeTypeCore.h"
#include <ft2build.h>
#include <freetype/freetype.h>
#include <QDebug>

FreeTypeCore::FreeTypeCore(QObject* parent)
    :QObject(parent)
{
    init();
}

FreeTypeCore::~FreeTypeCore()
{

}

void FreeTypeCore::init(void)
{
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        qDebug() << "Init FreeType Library Error!";
}

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

bool FreeTypeCore::loadChar(unsigned int unicode, QString str)
{
    m_renderString = str;
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

//    loadCurrentFace();


    m_path.clear();
    m_PointInfos.clear();

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

    if (!m_currentPointInfo.isEmpty())
        m_PointInfos.push_back(m_currentPointInfo);
    m_currentPointInfo.clear();

    return true;
}

void FreeTypeCore::getCurrentPointInfos(QVector<PointInfos>& pointInfos)
{
    pointInfos = m_PointInfos;
}

void FreeTypeCore::loadCurrentFace(void)
{
    FT_Outline outline = m_pFace->glyph->outline;
    m_nGlyphWidth = m_pFace->glyph->metrics.width;
    m_nGlyphHeight = m_pFace->glyph->metrics.height;

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

void FreeTypeCore::drawContour(int index, QPainterPath& tempPath, QPainter* painter)
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
            {
                path.quadTo(pointInfos[i - 1].pos, pointInfos[i].pos);
            }
            else if (i - 3 == nLastDrawedIndex)
            {
                QVector<QPointF> posVecs;
                posVecs << pointInfos[nLastDrawedIndex].pos << pointInfos[i - 2].pos << pointInfos[i - 1].pos << pointInfos[i].pos;
                bezierCurve(path, posVecs);
//                path.cubicTo(pointInfos[i - 2].pos, pointInfos[i - 1].pos, pointInfos[i].pos);
            }
            else if (i - 4 == nLastDrawedIndex)
            {
                QVector<QPointF> posVecs;
                posVecs << pointInfos[nLastDrawedIndex].pos << pointInfos[i - 3].pos \
                        << pointInfos[i - 2].pos << pointInfos[i - 1].pos << pointInfos[i].pos;

                bezierCurve(path, posVecs);
//                biquadTo(path, pointInfos[nLastDrawedIndex].pos, pointInfos[i - 3].pos, \
//                        pointInfos[i - 2].pos, pointInfos[i - 1].pos, pointInfos[i].pos);
            }
            else
            {
                QVector<QPointF> posVecs;
                posVecs << pointInfos[nLastDrawedIndex].pos;
                while (nLastDrawedIndex != i)
                {
                    posVecs << pointInfos[++nLastDrawedIndex].pos;
                }

                bezierCurve(path, posVecs);
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

    tempPath = path;
}

void FreeTypeCore::drawHandle(QPainter* painter)
{
    painter->save();

    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(255, 0, 0));
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    int nLastDrawedIndex = 0;
    for (auto iter = m_PointInfos.begin(); iter != m_PointInfos.end(); ++iter)
    {
        for (int i=0; i<iter->size(); ++i)
        {
            if (i == 0 && iter->at(i).pointType == 0)
            {
                painter->drawPoint(iter->at(i).pos);
                nLastDrawedIndex = 0;
                continue;
            }

            if (iter->at(i).pointType == 0)
            {
                if (i - 1 == nLastDrawedIndex)
                    painter->drawPoint(iter->at(i).pos);
                else if (i - 2 == nLastDrawedIndex)
                {
                    painter->drawLine(iter->at(nLastDrawedIndex).pos, iter->at(i - 1).pos);
                    painter->drawLine(iter->at(i - 1).pos, iter->at(i).pos);
                    painter->drawPoint(iter->at(i - 1).pos);
                }
                else if (i - 3 == nLastDrawedIndex)
                {
                    painter->drawLine(iter->at(nLastDrawedIndex).pos, iter->at(i - 2).pos);
                    painter->drawLine(iter->at(i - 1).pos, iter->at(i).pos);

                    painter->drawPoint(iter->at(i - 2).pos);
                    painter->drawPoint(iter->at(i - 1).pos);
                }

                nLastDrawedIndex = i;
            }

            if (i == iter->size() - 1)
            {
                if (iter->at(i).pointType == 0)
                    painter->drawPoint(iter->at(i).pos);
                else
                {
                    if (i - 1 == nLastDrawedIndex)
                    {
                        painter->drawLine(iter->at(nLastDrawedIndex).pos, iter->at(i).pos);
                        painter->drawLine(iter->at(i).pos, iter->at(0).pos);

                        painter->drawPoint(iter->at(i).pos);
                    }
                    else if (i - 2 == nLastDrawedIndex)
                    {
                        painter->drawLine(iter->at(nLastDrawedIndex).pos, iter->at(i - 1).pos);
                        painter->drawLine(iter->at(i).pos, iter->at(0).pos);

                        painter->drawPoint(iter->at(i - 1).pos);
                        painter->drawPoint(iter->at(i).pos);
                    }
                }
            }
        }
    }

    painter->restore();
}

void FreeTypeCore::render(QPainter* painter)
{
    QPen pen;
    pen.setWidth(1);
    pen.setColor(QColor(120, 120, 120));
    painter->setPen(pen);
    painter->drawPath(m_path);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->translate(QPoint(0, 120));
    QVector<QColor> nColorVec;
    nColorVec << QColor(255, 0, 0) << QColor(0, 255, 0) << QColor(0, 0, 255) << \
                 QColor(255, 255, 0) << QColor(0, 255, 255) << QColor(255, 0, 255) << \
                 QColor(0, 0, 0) << QColor(128, 0, 128) << QColor(128, 128, 255);

    QPainterPath tempPath;
    for (int i=0; i<m_PointInfos.size(); ++i)
    {
//        if (i != 0)
//            continue;
        QPen pen;
        pen.setWidth(1);
        pen.setColor(QColor(0, 0, 255));
        painter->setPen(pen);
        //painter->setBrush(QBrush(QColor(255, 0, 255)));

        QPainterPath path;
        drawContour(i, path, painter);
        tempPath.addPath(path);
//        painter->drawPath(tempPath);
    }
    painter->drawPath(tempPath);


    // Draw Handle
    drawHandle(painter);

    painter->restore();
}

void FreeTypeCore::biquadTo(QPainterPath& path, QPointF start, QPointF c1, QPointF c2, QPointF c3, QPointF to)
{

    for (int i=0; i<=100; ++i)
    {
        float t = i * 1.0 / 100;
        QPointF t1 = pow((1 - t), 4) * start;
        QPointF t2 = 4 * t * pow(1-t, 3) * c1;
        QPointF t3 = 6 * pow(t, 2) * pow(1-t, 2) * c2;
        QPointF t4 = 4 * pow(t, 3) * pow(1-t, 1) * c3;
        QPointF t5 = pow(t, 4) * to;

        QPointF destPos = t1 + t2 + t3 + t4 + t5;
        path.lineTo(destPos);
    }
}

void FreeTypeCore::bezierCurve(QPainterPath& path, const QVector<QPointF>& vec)
{
    int nCount = 10;
    for (int i=0; i<=nCount; ++i)
    {
        float t = i * 1.0 / nCount;

        QPointF destPos(0.0, 0.0);
        int n = vec.size() - 1;
        for (int j=0; j<vec.size(); ++j)
        {
            float f1 = factorial(n) / (factorial(j) * factorial(n - j));
            float f2 = pow(t, j) * pow(1-t, n - j);

            QPointF tempPos = f1 * f2 * vec[j];
            destPos += tempPos;
        }

        path.lineTo(destPos);
//        path.poin
    }
}

float FreeTypeCore::factorial(int number)
{
    if (number <= 0)
        return 1;

    float result = number;
    while (number > 1)
    {
        result *= (--number);
    }

    return result;
}

int FreeTypeCore::moveTo(const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.moveTo(QPoint(to->x / 64.0f, to->y / -64.0f));
    qDebug() << __FUNCTION__ << to->x << ", " << to->y;

    if (!pCore->m_currentPointInfo.isEmpty())
    {
        pCore->m_PointInfos.push_back(pCore->m_currentPointInfo);
    }
    pCore->m_currentPointInfo.clear();
    PointInfo pointInfo;
    pointInfo.pos.setX(to->x / 64.0f);
    pointInfo.pos.setY(to->y / -64.0f);
    pointInfo.pointType = 0;
    pCore->m_currentPointInfo.push_back(pointInfo);

    return 0;
}

int FreeTypeCore::lineTo(const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.lineTo(QPoint(to->x / 64.0f, to->y / -64.0f));
    qDebug() << __FUNCTION__ << to->x << ", " << to->y;

    PointInfo pointInfo;
    pointInfo.pos.setX(to->x / 64.0f);
    pointInfo.pos.setY(to->y / -64.0f);
    pointInfo.pointType = 0;
    pCore->m_currentPointInfo.push_back(pointInfo);

    return 0;
}

int FreeTypeCore::conicTo(const FT_Vector* control, const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.quadTo(QPoint(control->x / 64.0f, control->y / -64.0f), QPoint(to->x / 64.0f, to->y / -64.0f));
    qDebug() << __FUNCTION__ << control->x << ", " << control->y << "; " << to->x << ", " << to->y;

    PointInfo pointInfo;
    pointInfo.pos.setX(control->x / 64.0f);
    pointInfo.pos.setY(control->y / -64.0f);
    pointInfo.pointType = 1;
    pCore->m_currentPointInfo.push_back(pointInfo);

    pointInfo.pos.setX(to->x / 64.0f);
    pointInfo.pos.setY(to->y / -64.0f);
    pointInfo.pointType = 0;
    pCore->m_currentPointInfo.push_back(pointInfo);

    return 0;
}

int FreeTypeCore::cubicto(const FT_Vector* control1, const FT_Vector* control2, const FT_Vector* to, void* user)
{
    FreeTypeCore* pCore = (FreeTypeCore*)user;
    pCore->m_path.cubicTo(QPoint(control1->x / 64.0f, control1->y / -64.0f), \
                          QPoint(control2->x / 64.0f, control2->y / -64.0f), \
                          QPoint(to->x / 64.0f, to->y / 64.0f));

    qDebug() << __FUNCTION__ << control1->x << ", " << control1->y << "; " \
                             << control2->x << ", " << control2->y << "; " \
                             << to->x << ", " << to->y;

    PointInfo pointInfo;
    pointInfo.pos.setX(control1->x / 64.0f);
    pointInfo.pos.setY(control1->y / -64.0f);
    pointInfo.pointType = 1;
    pCore->m_currentPointInfo.push_back(pointInfo);

    pointInfo.pos.setX(control2->x / 64.0f);
    pointInfo.pos.setY(control2->y / -64.0f);
    pointInfo.pointType = 1;
    pCore->m_currentPointInfo.push_back(pointInfo);

    pointInfo.pos.setX(to->x / 64.0f);
    pointInfo.pos.setY(to->y / -64.0f);
    pointInfo.pointType = 0;
    pCore->m_currentPointInfo.push_back(pointInfo);

    return 0;
}

#include "HPNearCharacterCore.h"
#include <QFile>
#include <QDebug>

HPNearCharactorCore::HPNearCharactorCore(QObject* parent)
    :QObject(parent)
{
    m_ideographsTypeMap.clear();
    m_wordSpliteInfoMaps.clear();
    m_wordRadicalInfoMaps.clear();
}

HPNearCharactorCore::~HPNearCharactorCore()
{

}

HPNearCharactorCore* HPNearCharactorCore::getInstance(void)
{
    static HPNearCharactorCore instance;
    return &instance;
}

void HPNearCharactorCore::init(void)
{
    fillIdeographsTypeMap();
    loadWordSpliteFile(":/Resource/WordSplit.txt");
}

void HPNearCharactorCore::loadWordSpliteFile(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly))
        return;

    int nBufferSize = 1000;
    char* pStringBuffer = new char[nBufferSize];
    memset(pStringBuffer, 0, nBufferSize);
    int nLength = file.readLine(pStringBuffer, nBufferSize);

    while (nLength > 0)
    {
        processWordSplit(pStringBuffer, nBufferSize);

        memset(pStringBuffer, 0, nBufferSize);
        nLength = file.readLine(pStringBuffer, nBufferSize);
    }
}

void HPNearCharactorCore::processWordSplit(char* pBuffer, int size)
{
    QString string = QString::fromUtf8(pBuffer);

    QStringList strList = string.split("\t");
    int nCount = 0;
    HPWordInfo wordInfo;

    for (auto iter = strList.begin(); iter != strList.end(); ++iter)
    {
        QString tempString = *iter;
        tempString = tempString.simplified();

        // Get Key Unicode
        int nStringSize = tempString.length();
        if (nCount == 0)
        {
            wordInfo.unicode = tempString.unicode()[0].unicode();
            nCount++;
            continue;
        }
        nCount++;

        // Append to splitUnicodes
        QVector<quint32> interUnicodes;
        for (int i=0; i<nStringSize; ++i)
        {
            interUnicodes << tempString.unicode()[i].unicode();
        }
        wordInfo.splitUnicodes.push_back(interUnicodes);
    }

    // Fill To word radical info map
    m_wordRadicalInfoMaps.insert(wordInfo.unicode, wordInfo);
    // fill To m_wordSpliteInfoMaps
    syncToWordSpliteInfoMap(wordInfo);
}

void HPNearCharactorCore::syncToWordSpliteInfoMap(const HPWordInfo& info)
{
    int nSize = info.splitUnicodes.size();

    for (int i=0; i<nSize; ++i)
    {
        QVector<quint32> unicodeVec = info.splitUnicodes[i];

        for (int j=0; j<unicodeVec.size(); ++j)
        {
            if (m_ideographsTypeMap.find(unicodeVec[j]) != m_ideographsTypeMap.end())
                continue;

            auto iter = m_wordSpliteInfoMaps.find(unicodeVec[j]);
            if (iter != m_wordSpliteInfoMaps.end())
            {
                bool hasThis = false;
                for (int k = 0; k < iter->size(); ++k)
                {
                    if (iter->at(k) == info.unicode)
                    {
                        hasThis = true;
                        break;
                    }
                }

                if (!hasThis)
                    iter->append(info.unicode);
            }
            else
            {
                QVector<quint32> unicodeVecs;
                unicodeVecs << info.unicode;
                m_wordSpliteInfoMaps.insert(unicodeVec[j], unicodeVecs);
            }
        }
    }
}

void HPNearCharactorCore::fillIdeographsTypeMap(void)
{
    m_ideographsTypeMap.insert(12272, t_LeftToRight);
    m_ideographsTypeMap.insert(12273, t_TopToBottom);
    m_ideographsTypeMap.insert(12274, t_LeftToRight3);
    m_ideographsTypeMap.insert(12275, t_TopToBottom3);
    m_ideographsTypeMap.insert(12276, t_InsetToOutset);
    m_ideographsTypeMap.insert(12277, t_BottomOpen3);
    m_ideographsTypeMap.insert(12278, t_TopOpen3);
    m_ideographsTypeMap.insert(12279, t_RightOpen3);
    m_ideographsTypeMap.insert(12280, t_TopLeftToBottomRight2Open);
    m_ideographsTypeMap.insert(12281, t_TopRightToBottomLeft2Open);
    m_ideographsTypeMap.insert(12282, t_BottomLeftToTopRight2Open);
    m_ideographsTypeMap.insert(12283, t_OverLap);
}

QVector<quint32> HPNearCharactorCore::findWordByRadicals(quint64 unicode)
{
    if (m_wordSpliteInfoMaps.find(unicode) == m_wordSpliteInfoMaps.end())
        return QVector<quint32>();

    return m_wordSpliteInfoMaps[unicode];
}

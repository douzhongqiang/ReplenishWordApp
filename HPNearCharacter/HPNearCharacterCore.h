#ifndef HPNEARCHARACTORCORE_H
#define HPNEARCHARACTORCORE_H

#include <QObject>
#include <QMap>
#include "HPNearCharacter_global.h"

#define g_HPNearCharactorCore HPNearCharactorCore::getInstance()

class HPNEARCHARACTER_EXPORT HPNearCharactorCore : public QObject
{
    Q_OBJECT

public:
    struct HPWordInfo
    {
        quint32 unicode;

        QVector<QVector<quint32>> splitUnicodes;
    };

    enum IdeographsType
    {
        t_LeftToRight,                  // U+2FF0 ⿰
        t_TopToBottom,                  // U+2FF1 ⿱
        t_LeftToRight3,                 // U+2FF2 ⿲
        t_TopToBottom3,                 // U+2FF3 ⿳
        t_InsetToOutset,                // U+2FF4 ⿴
        t_BottomOpen3,                  // U+2FF5 ⿵
        t_TopOpen3,                     // U+2FF6 ⿶
        t_RightOpen3,                   // U+2FF7 ⿷
        t_TopLeftToBottomRight2Open,    // U+2FF8 ⿸
        t_TopRightToBottomLeft2Open,    // U+2FF9 ⿹
        t_BottomLeftToTopRight2Open,    // U+2FFA ⿺
        t_OverLap                       // U+2FFB ⿻
    };

public:
    static HPNearCharactorCore* getInstance(void);

    HPNearCharactorCore(QObject* parent = nullptr);
    ~HPNearCharactorCore();

    void init(void);
    void loadWordSpliteFile(const QString& filename);

    QVector<quint32> findWordByRadicals(quint64 unicode);

private:
    QMap<quint32, HPWordInfo> m_wordRadicalInfoMaps;
    QMap<quint32, QVector<quint32>> m_wordSpliteInfoMaps;
    QMap<quint32, IdeographsType>   m_ideographsTypeMap;

    void processWordSplit(char* pBuffer, int size);
    void syncToWordSpliteInfoMap(const HPWordInfo& info);
    void fillIdeographsTypeMap(void);
};

#endif

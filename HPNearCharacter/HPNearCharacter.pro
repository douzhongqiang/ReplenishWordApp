QT += core gui widgets

TEMPLATE = lib
DEFINES += HPNEARCHARACTER_LIBRARY

CONFIG += c++11

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../ReplenishWord/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    HPNearCharacterCore.cpp \
    hpnearcharacter.cpp

HEADERS += \
    HPNearCharacterCore.h \
    HPNearCharacter_global.h \
    hpnearcharacter.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc

DISTFILES += \
    Resource/GBK-拆分.txt

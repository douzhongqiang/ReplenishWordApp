QT += core gui widgets

TEMPLATE = lib
DEFINES += FREETYPEWRAPER_LIBRARY

CONFIG += c++11

MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/../ReplenishWord/bin

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../3part/FreeType/include
CONFIG(debug, debug|release) {
    #VS2019
    LIBS += -L$$PWD/../3part/FreeType/libs/Debug -lfreetyped
}
else{
    LIBS += -L$$PWD/../3part/FreeType/libs/Release -lfreetype
}

SOURCES += \
    FreeTypeConfig.cpp \
    FreeTypeCore.cpp \
    FreeTypeGlyphItem.cpp \
    FreeTypeOperator.cpp \
    FreeTypeRenderWidget.cpp \
    FreeTypeSelectedItem.cpp \
    freetypewraper.cpp

HEADERS += \
    FreeTypeConfig.h \
    FreeTypeCore.h \
    FreeTypeGlyphItem.h \
    FreeTypeOperator.h \
    FreeTypeRenderWidget.h \
    FreeTypeSelectedItem.h \
    FreeTypeWraper_global.h \
    freetypewraper.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

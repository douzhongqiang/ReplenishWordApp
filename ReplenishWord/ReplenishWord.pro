QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

MOC_DIR += $$PWD/temp
DESTDIR += $$PWD/bin
OBJECTS_DIR += $$PWD/temp

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../FreeTypeWraper
LIBS += -L$$PWD/bin/ -lFreeTypeWraper

SOURCES += \
    ReplenishWordMainWidget.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    ReplenishWordMainWidget.h \
    mainwindow.h

INCLUDEPATH += $$PWD/../3part/FreeType/include
CONFIG(debug, debug|release) {
    #VS2019
    LIBS += -L$$PWD/../3part/FreeType/libs/Debug -lfreetyped
}
else{
    LIBS += -L$$PWD/../3part/FreeType/libs/Release -lfreetype
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resource.qrc


QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MeshViewQt
TEMPLATE = app

win32 {
    LIBS+=-lopengl32
    LIBS+=-lglu32
}

SOURCES += main.cpp\
        window.cpp \
    trianglemesh.cpp \
    mfileparser.cpp \
    viewportwidget.cpp \
    parseworker.cpp \
    leaplistener.cpp \
    handmotionconverter.cpp

HEADERS  += window.h \
    trianglemesh.h \
    mfileparser.h \
    viewportwidget.h \
    parseworker.h \
    leaplistener.h \
    handmotionconverter.h \
    commons.h

FORMS    += window.ui

RESOURCES += \
    meshviewer_resources.qrc

macx: LIBS += -L$$PWD/../../LeapSDK/lib/ -lLeap

INCLUDEPATH += $$PWD/../../LeapSDK/include
DEPENDPATH += $$PWD/../../LeapSDK/include

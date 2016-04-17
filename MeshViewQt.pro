
QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MeshViewQt
TEMPLATE = app

win32 {
    LIBS+=-lopengl32
    LIBS+=-lglu32
    LIBS += -L$$PWD/../../LeapSDK/lib/x64/ -lLeap

    INCLUDEPATH += $$PWD/../../LeapSDK/include
    DEPENDPATH += $$PWD/../../LeapSDK/include
}

SOURCES += main.cpp\
        window.cpp \
    trianglemesh.cpp \
    mfileparser.cpp \
    viewportwidget.cpp \
    parseworker.cpp \
    leaplistener.cpp \
    leapaction.cpp \
    leapactionsender.cpp

HEADERS  += window.h \
    trianglemesh.h \
    mfileparser.h \
    viewportwidget.h \
    parseworker.h \
    leaplistener.h \
    commons.h \
    leapaction.h \
    leapactionsender.h

FORMS    += window.ui

RESOURCES += \
    meshviewer_resources.qrc

macx {
     LIBS += -L$$PWD/../../LeapSDK/lib/ -lLeap

    INCLUDEPATH += $$PWD/../../LeapSDK/include
    DEPENDPATH += $$PWD/../../LeapSDK/include
}


QT += core gui opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple_tetrahedron
TEMPLATE = app

win32:LIBS += -lopengl32
linux:LIBS += -lGL

SOURCES += \
    main.cc \
    widget.cc

HEADERS += \
    widget.h

RESOURCES += \
    resources.qrc

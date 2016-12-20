QT += core gui opengl

CONFIG += c++11

win32: LIBS += -lopengl32
linux: LIBS += -lGL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = particle_system
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cc\
    scene_base.cc \
    smoke_scene.cc

HEADERS += \
    scene_base.h \
    smoke_scene.h \
    help_math.h

RESOURCES += resources.qrc


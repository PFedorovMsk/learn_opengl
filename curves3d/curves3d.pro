QT += core gui opengl

CONFIG += c++11

win32: LIBS += -lopengl32
linux: LIBS += -lGL

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = curves3d
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += resources.qrc

FORMS += \
    form.ui

HEADERS += \
    bezier.h \
    bspline.h \
    curve3d.h \
    form.h \
    nurbs.h \
    scene.h \
    scene_base.h

SOURCES += \
    bezier.cc \
    bspline.cc \
    curve3d.cc \
    form.cc \
    main.cc \
    nurbs.cc \
    scene.cc \
    scene_base.cc


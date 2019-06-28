QT += core gui multimedia concurrent

CONFIG += qt warn_on

include($$PWD/zxing-cpp.pri)

INCLUDEPATH  += $$PWD

HEADERS += \
    $$PWD/qzxingnu.h \
    $$PWD/qzxingnufilter.h

SOURCES += \
    $$PWD/qzxingnu.cpp \
    $$PWD/qzxingnufilter.cpp

######################################################################
# Automatically generated by qmake (3.0) Sun Nov 1 11:21:24 2015
######################################################################

target.path = /usr/local/bin

TEMPLATE = app
TARGET = eyeguard
INCLUDEPATH += . src src/common
INSTALLS += target
TRANSLATIONS = translations/russian.ts

greaterThan (QT_MAJOR_VERSION, 4): QT += widgets

# Input
HEADERS += src/defs.h \
           src/eyeguard.h \
           src/settingsdialog.h \
           src/version.h \
           src/common/qtimeoutslider.h
SOURCES += src/eyeguard.cpp \
           src/main.cpp \
           src/settingsdialog.cpp \
           src/common/qtimeoutslider.cpp
RESOURCES += eyeguard.qrc
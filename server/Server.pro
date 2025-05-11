QT += core gui network widgets

TARGET = Server
TEMPLATE = app

CONFIG += c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    server.cpp

HEADERS += \
    mainwindow.h \
    server.h

FORMS += \
    mainwindow.ui

# Дополнительные настройки
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += $$QMAKE_CFLAGS_WARN_ON

TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -pthread

SOURCES += main.cpp \
    messagequeue.cpp \
    messagesender.cpp \
    server.cpp \
    connection.cpp \
    message.cpp

HEADERS += \
    threadloop.h \
    messagequeue.h \
    message.h \
    messagesender.h \
    server.h \
    connection.h \
    json.hpp

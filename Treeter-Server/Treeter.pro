TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -pthread
LIBS += -lcrypto

SOURCES += main.cpp \
    server.cpp \
    connection.cpp \
    config.cpp \
    connectionlist.cpp \
    crypto/base64.cpp \
    crypto/crypto.cpp \
    message/messageincoming.cpp \
    message/messageoutgoing.cpp \
    message/messageprocessor.cpp \
    message/messagequeue.cpp \
    message/messagesender.cpp

HEADERS += \
    threadloop.h \
    server.h \
    connection.h \
    config.h \
    connectionlist.h \
    crypto/base64.h \
    crypto/crypto.h \
    message/messagebase.h \
    message/messageincoming.h \
    message/messageincomingctor.h \
    message/messageoutgoing.h \
    message/messageprocessor.h \
    message/messagequeue.h \
    message/messagesender.h \
    message/json.hpp

Debug:DESTDIR = debug

# -------------------------------------------------
# Project created by QtCreator 2010-06-29T15:59:32
# -------------------------------------------------
QT += network
QT -= gui
TARGET = OSCms
TEMPLATE = lib
DEFINES += OSCMS_LIBRARY
SOURCES += OSCPort.cpp \
    OSCPacketDispatcher.cpp \
    OSCPacket.cpp \
    OSCMessage.cpp \
    OSCBundle.cpp \
    OSCByteArrayToMsgConverter.cpp \
    OSCMsgToByteArrayConverter.cpp
HEADERS += OSCms_global.h \
    OSCPort.h \
    OSCPacketDispatcher.h \
    OSCPacket.h \
    OSCMessage.h \
    OSCListener.h \
    OSCBundle.h \
    OSCByteArrayToMsgConverter.h \
    OSCMsgToByteArrayConverter.h

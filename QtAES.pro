QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -msse2 -msse -march=native -maes
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    document/buffer/qfilebuffer.cpp \
    document/buffer/qhexbuffer.cpp \
    document/buffer/qmemorybuffer.cpp \
    document/buffer/qmemoryrefbuffer.cpp \
    document/commands/hexcommand.cpp \
    document/commands/insertcommand.cpp \
    document/commands/removecommand.cpp \
    document/commands/replacecommand.cpp \
    document/qhexcursor.cpp \
    document/qhexdocument.cpp \
    document/qhexmetadata.cpp \
    document/qhexrenderer.cpp \
    main.cpp \
    mainwindow.cpp \
    qhexview.cpp

HEADERS += \
    aes.h \
    aes_ni.h \
    aes_step.h \
    aes_table.h \
    document/buffer/qfilebuffer.h \
    document/buffer/qhexbuffer.h \
    document/buffer/qmemorybuffer.h \
    document/buffer/qmemoryrefbuffer.h \
    document/commands/hexcommand.h \
    document/commands/insertcommand.h \
    document/commands/removecommand.h \
    document/commands/replacecommand.h \
    document/qhexcursor.h \
    document/qhexdocument.h \
    document/qhexmetadata.h \
    document/qhexrenderer.h \
    mainwindow.h \
    qhexview.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    QtAES_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

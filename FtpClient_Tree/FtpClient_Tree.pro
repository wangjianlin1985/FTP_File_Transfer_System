#-------------------------------------------------
#
# Project created by QtCreator 2016-12-28T09:27:18
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FtpClient_Tree
TEMPLATE = app


SOURCES += main.cpp \
    FileNameWidget.cpp \
    FileTool.cpp \
    ftpclient.cpp \
    serverfilestreeview.cpp \
    QtFtp/qftp.cpp \
    QtFtp/qurlinfo.cpp

HEADERS  += \
    FileNameWidget.h \
    FileTool.h \
    ftpclient.h \
    serverfilestreeview.h \
    QtFtp/qftp.h \
    QtFtp/qurlinfo.h

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/QtFtp/release/ -lQt5Ftp
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/QtFtp/debug/ -lQt5Ftp
#else:unix: LIBS += -L$$PWD/QtFtp/ -lQt5Ftp

#INCLUDEPATH += $$PWD/QtFtp
#DEPENDPATH += $$PWD/QtFtp

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/QtFtp/release/libQt5Ftp.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/QtFtp/debug/libQt5Ftp.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/QtFtp/release/Qt5Ftp.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/QtFtp/debug/Qt5Ftp.lib
#else:unix: PRE_TARGETDEPS += $$PWD/QtFtp/libQt5Ftp.a

RESOURCES += \
    source.qrc

FORMS += \
    FileNameWidget.ui

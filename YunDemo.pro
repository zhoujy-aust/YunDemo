QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    buttongroup.cpp \
    common/common.cpp \
    common/des.c \
    common/downloadlayout.cpp \
    common/downloadtask.cpp \
    common/logininfoinstance.cpp \
    common/uploadlayout.cpp \
    common/uploadtask.cpp \
    login.cpp \
    main.cpp \
    mainwindow.cpp \
    myfilewg.cpp \
    rankinglist.cpp \
    selfwidget/dataprogress.cpp \
    selfwidget/filepropertyinfo.cpp \
    selfwidget/mymenu.cpp \
    sharelist.cpp \
    title_wg.cpp \
    transfer.cpp

HEADERS += \
    buttongroup.h \
    common/common.h \
    common/des.h \
    common/downloadlayout.h \
    common/downloadtask.h \
    common/logininfoinstance.h \
    common/uploadlayout.h \
    common/uploadtask.h \
    login.h \
    mainwindow.h \
    myfilewg.h \
    rankinglist.h \
    selfwidget/dataprogress.h \
    selfwidget/filepropertyinfo.h \
    selfwidget/mymenu.h \
    sharelist.h \
    title_wg.h \
    transfer.h

FORMS += \
    buttongroup.ui \
    login.ui \
    mainwindow.ui \
    myfilewg.ui \
    rankinglist.ui \
    selfwidget/dataprogress.ui \
    selfwidget/filepropertyinfo.ui \
    sharelist.ui \
    title_wg.ui \
    transfer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    src.qrc

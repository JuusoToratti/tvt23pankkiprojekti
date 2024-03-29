QT       += core gui
QT += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    addPin.cpp \
    main.cpp \
    mainuserinterface.cpp \
    mainwindow.cpp \
    moneyselect.cpp

HEADERS += \
    addPin.h \
    mainuserinterface.h \
    mainwindow.h \
    moneyselect.h

FORMS += \
    addPin.ui \
    mainuserinterface.ui \
    mainwindow.ui \
    moneyselect.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../cardreader/build/release/ -lcardreader
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../cardreader/build/debug/ -lcardreader

INCLUDEPATH += $$PWD/../cardreader
DEPENDPATH += $$PWD/../cardreader

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../database/build/release/ -ldatabase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../database/build/debug/ -ldatabase

INCLUDEPATH += $$PWD/../database
DEPENDPATH += $$PWD/../database

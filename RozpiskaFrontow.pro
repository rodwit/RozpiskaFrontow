QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17, printsupport

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

win32
{
    RC_ICONS = ProgramIcon.ico
}

SOURCES += \
    MainWindow/mainwindow.cpp \
    MainWindow/pagelist.cpp \
    MainWindow/pageorder.cpp \
    PrintManager/printmanager.cpp \
    PrintManager/printscheme.cpp \
    dialogfront.cpp \
    filedata.cpp \
    main.cpp \
    mytable.cpp

HEADERS += \
	FrontsDataStruct.h \
	MainWindow/mainwindow.h \
	MainWindow/pagelist.h \
	MainWindow/pageorder.h \
	PrintManager/printmanager.h \
	PrintManager/printscheme.h \
	dialogfront.h \
	filedata.h \
	mytable.h

FORMS += \
	MainWindow/mainwindow.ui \
	MainWindow/pagelist.ui \
	MainWindow/pageorder.ui \
	PrintManager/printmanager.ui \
	PrintManager/printscheme.ui \
	dialogfront.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES +=

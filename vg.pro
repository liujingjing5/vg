#-------------------------------------------------
#
# Project created by QtCreator 2018-06-01T23:45:45
#
#-------------------------------------------------

QT       += core gui qml
RC_ICONS = vg1.ico
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = vg
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    common/utils.cpp \
    common/script_manager.cpp \
    common/template_manager.cpp \
    common/flowlayout.cpp \
    common/rotationtoolbutton.cpp \
    qrunnableplus.cpp \
    main.cpp \
    mainwindow.cpp \
    project_maker_window.cpp \
    project_maker/code_editor.cpp \
    common/file_type/file_type_utils.cpp


HEADERS += \
    common/macro_tools.h \
    common/sg.h \
    common/utils.h \
    common/script_manager.h \
    common/template_manager.h \
    common/flowlayout.h \
    common/rotationtoolbutton.h \
    qrunnableplus.h \
    mainwindow.h \
    project_maker_window.h \
    project_maker/code_editor.h \
    project_maker/ieditor.h \
    common/file_type/file_type_utils.h

FORMS += \
        mainwindow.ui \
    project_maker_window.ui

DISTFILES +=

RESOURCES += \
    asserts.qrc



CONFIG += qscintilla2

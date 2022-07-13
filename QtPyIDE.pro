QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editor/code_editor.cpp \
    editor/console_view.cpp \
    editor/files_view.cpp \
    editor/input_line.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    editor/code_editor.h \
    editor/console_view.h \
    editor/files_view.h \
    editor/input_line.h \
    mainwindow.h

TRANSLATIONS += \
    QtPyIDE_tr_TR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

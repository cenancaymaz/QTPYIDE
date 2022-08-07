QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    editor/code_editor/code_editor.cpp \
    editor/code_editor/line_number_area.cpp \
    editor/code_editor/python_syntax_highlighter.cpp \
    editor/code_editor/single_editor.cpp \
    editor/code_manager/code_manager.cpp \
    editor/console_view/console_view.cpp \
    editor/console_view/input_line.cpp \
    editor/console_view/python_process.cpp \
    editor/console_view/single_console.cpp \
    editor/files_view/files_view.cpp \
    main.cpp \
    mainwindow.cpp \
    util/borderless_button.cpp \
    util/startup_settings.cpp

HEADERS += \
    editor/code_editor/code_editor.h \
    editor/code_editor/line_number_area.h \
    editor/code_editor/python_syntax_highlighter.h \
    editor/code_editor/single_editor.h \
    editor/code_manager/code_manager.h \
    editor/console_view/console_view.h \
    editor/console_view/input_line.h \
    editor/console_view/python_process.h \
    editor/console_view/single_console.h \
    editor/files_view/files_view.h \
    mainwindow.h \
    util/borderless_button.h \
    util/startup_settings.h

TRANSLATIONS += \
    QtPyIDE_tr_TR.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    icons.qrc

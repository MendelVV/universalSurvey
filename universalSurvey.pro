#-------------------------------------------------
#
# Project created by QtCreator 2015-10-05T19:43:22
#
#-------------------------------------------------

QT       += core gui xml sql axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 2.1.2.6
RC_ICONS = tasks.ico

TARGET = universalSurvey
TEMPLATE = app

DEPENDPATH+=D:/My_dll_library/headers/mkform
INCLUDEPATH+=D:/My_dll_library/headers/mkform

LIBS+=-LD:/My_dll_library/qt_5.5 -lmkform1

DEPENDPATH+=D:/My_dll_library/headers/surveydata
INCLUDEPATH+=D:/My_dll_library/headers/surveydata

LIBS+=-LD:/My_dll_library/qt_5.5 -lsurveydata1

DEPENDPATH+=D:/My_dll_library/headers/qexcel
INCLUDEPATH+=D:/My_dll_library/headers/qexcel

LIBS+=-LD:/My_dll_library/qt_5.5 -lqexceldll1

SOURCES += main.cpp \
    dialogs/dialogaddclass.cpp \
    dialogs/dialogaddstudent.cpp \
    dialogs/dialogyousure.cpp \
    widgets/classwidget.cpp \
    widgets/disciplinewidget.cpp \
    widgets/mainwindow.cpp \
    widgets/miniview.cpp \
    widgets/navigator.cpp \
    widgets/schoolwidget.cpp \
    widgets/settingwidget.cpp \
    widgets/studentwidget.cpp \
    widgets/tableclasswidget.cpp \
    widgets/tabledisciplinewidget.cpp \
    widgets/tablestudentwidget.cpp \
    styleclass.cpp \
    database/dbschema.cpp \
    database/dbhelper.cpp \
    dialogs/dialogselectontable.cpp \
    dialogs/errormessage.cpp \
    dialogs/dialogclose.cpp \
    excelfunction.cpp \
    json/jsonhelper.cpp \
    keys/keyhelper.cpp \
    keys/allkeyclass.cpp \
    dialogs/dialogselectonlist.cpp \
    filehelper.cpp \
    keys/resultsdata.cpp

HEADERS  += \
    dialogs/dialogaddclass.h \
    dialogs/dialogaddstudent.h \
    dialogs/dialogyousure.h \
    widgets/classwidget.h \
    widgets/disciplinewidget.h \
    widgets/mainwindow.h \
    widgets/miniview.h \
    widgets/navigator.h \
    widgets/schoolwidget.h \
    widgets/settingwidget.h \
    widgets/studentwidget.h \
    widgets/tableclasswidget.h \
    widgets/tabledisciplinewidget.h \
    widgets/tablestudentwidget.h \
    styleclass.h \
    database/dbschema.h \
    database/dbhelper.h \
    dialogs/dialogselectontable.h \
    dialogs/errormessage.h \
    dialogs/dialogclose.h \
    excelfunction.h \
    json/jsonhelper.h \
    keys/keyhelper.h \
    keys/allkeyclass.h \
    dialogs/dialogselectonlist.h \
    filehelper.h \
    keys/resultsdata.h

RESOURCES += \
    resource.qrc

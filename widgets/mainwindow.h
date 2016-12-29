#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include "schoolwidget.h"
#include "studentwidget.h"
#include "navigator.h"
#include "settingclass.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    QMdiArea* mdiArea;//главная рабочая область
    Navigator* nav;
protected:
    double x;
    double y;
    QAction* actCreate;
    QAction* actOpen;
    QAction* actOpenXML;
    QAction* actSave;
    QAction* actExit;

    QAction* actAuthor;

    void setActMenu();

signals:
    void signalSave();
public slots:
    void slotSave();
    void slotCreateSchool();
    void slotOpen();
    void slotOpenXML();

    void slotOpenClass(ClassClass*);
    void slotOpenStudent(StudentClass*);

    void slotCloseSubWindow();

    void slotActivateSubWindow();

    void slotAuthor();
};

#endif // MAINWINDOW_H

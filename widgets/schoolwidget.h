#ifndef SCHOOLWIDGET_H
#define SCHOOLWIDGET_H

#include <QtWidgets>
#include "layouttoform.h"
#include "schoolclass.h"
#include "tableclasswidget.h"
#include "dialogs\dialogaddclass.h"

class SchoolWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit SchoolWidget(SchoolClass* school,  QWidget *parent = 0);
    ~SchoolWidget();
    FormPlusData* form;
    SchoolClass* sch;
    QString fileName;
protected:
    virtual void closeEvent(QCloseEvent *);
    TableClassWidget* tcw;

    QAction* actSave;
    QAction* actSaveAs;
    QAction* actToXML;
    QAction* actExit;
    void setMainMenu();

    QAction* actAddClass;
    void setPropertyMenu();

signals:
    void closed();
    void signalOpenClass(ClassClass*);
    void signalActivate();
public slots:
    void slotOpenClass(ClassClass*);
    void slotClose();
    void slotSave();
    void slotSaveAs();
    void slotToXML();
    void slotRefresh();

    void slotAddClass();
    void slotRemoveClass();
    void slotActivate();

};

#endif // SCHOOLWIDGET_H

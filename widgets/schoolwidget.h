#ifndef SCHOOLWIDGET_H
#define SCHOOLWIDGET_H

#include <QtWidgets>
#include "layouttoform.h"
#include "schoolclass.h"
#include "tableclasswidget.h"
#include "dialogs\dialogaddclass.h"
#include "database/dbhelper.h"
#include "dialogs/dialogselectontable.h"

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
    QAction* actSaveOther;
    QAction* actToXML;
    QAction* actExit;
    void setMainMenu();

    QAction* actAddClass;
    void setPropertyMenu();

    QString getFileName();
signals:
    void closed();
    void signalSave();
    void signalOpenClass(ClassClass*);
    void signalActivate();
public slots:
    void slotSaveSignal();
    void slotOpenClass(ClassClass*);
    void slotClose();
    void slotSave();
    void slotSaveAs();
    void slotSaveOther();
    void slotToXML();
    void slotRefresh();

    void slotAddClass();
    void slotRemoveClass();
    void slotActivate();
    void slotSelectSchool();
};

#endif // SCHOOLWIDGET_H

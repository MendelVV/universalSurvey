#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include <QtWidgets>
#include "layouttoform.h"
#include "classclass.h"
#include "tablestudentwidget.h"
#include "dialogs\dialogaddstudent.h"
#include "dialogs\dialogyousure.h"
#include "tableformdata.h"
#include "scrollwidget.h"
#include "settingclass.h"

class ClassWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit ClassWidget(ClassClass* classC, int md=0, QWidget *parent = 0);
    ~ClassWidget();
    ClassClass* cls;
    FormPlusData* form;
protected:
    SettingClass* setting;
    void setEnterForm();
    double x;
    double y;
    int mode;//если 0 то это форма, если 1 - то это таблица
    QMap<QString, FormPlusData*> mapForm;
    virtual void closeEvent(QCloseEvent *);
    QTabWidget* tab;
    TableStudentWidget* tsw;

    QAction* actSave;
    QAction* actAddStudent;
    QAction* actClose;
    void setActMenu();

    QAction* actAddMainForm;

    QAction* actRefreshForms;
    QMap<QString, QAction*> mapActAdd;//карта всех добавляющихся форм. без главной вероятно
    void setActAddForm();

    void setFormMainEnter();

    void setForm(QString str);
    void setFormClassed(QString str);
signals:
    void closed();
    void signalOpenStudent(StudentClass*);
    void signalLoadStudent(StudentClass*);
    void signalActivate();
    void signalSave();

public slots:
    void slotClose();
    void slotSaveAndClose();
    void slotAddStudent();
    void slotRemoveStudent();
    void slotOpenStudent(StudentClass*);
    void slotActivate();
    void slotRefreshForms();

    void slotAddMainForm();

    void slotAddForm();

    void slotSave();
    void slotDulicate(QString);
};

#endif // CLASSWIDGET_H

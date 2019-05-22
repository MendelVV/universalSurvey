#ifndef CLASSWIDGET_H
#define CLASSWIDGET_H

#include <QtWidgets>
#include "layouttoform.h"
#include "classclass.h"
#include "tablestudentwidget.h"
#include "dialogs\dialogaddstudent.h"
#include "dialogs\dialogyousure.h"
#include "tableformwidget.h"
#include "tableformdata.h"
#include "scrollwidget.h"
#include "settingclass.h"
#include "dialogs/dialogselectonlist.h"
#include "keys/keyhelper.h"
#include "dialogs/errormessage.h"
#include "dialogs/dialogattention.h"

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
    TableStudentWidget* tsw=0;

    QAction* actSave;
    QAction* actAddStudent;
    QAction* actClose;
    QAction* actResults;
    QAction* actUpdateCompleted;
    void setActMenu();

    QAction* actAddMainForm;

    QAction* actRefreshForms;
    QMap<QString, QAction*> mapActAdd;//карта всех добавляющихся форм. без главной вероятно
    void setActAddForm();

    void setFormMainEnter();

    void setForm(QString str);
    void setFormClassed(QString str);

    QMap<QString, QWidget*> mMapWgt;//словарь уже добавленых виджетов
    QMap<QString, int> mMapWgtPos;//словарь их позиций
    ScrollWidget* mListWgt=0;
    QVector<QWidget*> mVecEmpty;


signals:
    void closed();
    void signalOpenStudent(StudentClass*);
    void signalLoadStudent(StudentClass*);
    void signalActivate();
    void signalSave();

public slots:

    void slotShow(int);

    void slotClose();
    void slotSaveAndClose();
    void slotAddStudent();
    void slotRemoveStudent();
    void slotOpenStudent(StudentClass*);
    void slotActivate();
    void slotRefreshForms();

    void slotAddMainForm();

    void slotAddForm();

    void slotSaveThis();
    void slotSave();
    void slotDulicate(QString);

    void slotResults();

    void slotUpdateCompleted();
};

#endif // CLASSWIDGET_H

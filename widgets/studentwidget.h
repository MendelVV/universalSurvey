#ifndef STUDENTWIDGET_H
#define STUDENTWIDGET_H

#include "layouttoform.h"
#include "scrollwidget.h"
#include "studentclass.h"

class StudentWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit StudentWidget(StudentClass* student, QWidget *parent = 0);
    ~StudentWidget();
    FormPlusData* form;
    StudentClass* stud;
protected:
    QTabWidget* mTab;
    void setForms();//задаем формы любые. вход, выход, просто
    virtual void closeEvent(QCloseEvent *);
    QMap<QString, FormPlusData*> mapForm;

    QAction* actSave;
    QAction* actClose;
    QAction* actSaveAndClose;

    void setActMenu();

    void setForm(QString);
    QMap<QString, QWidget*> mMapWgt;
    QMap<QString, int> mMapWgtPos;
    QVector<QWidget*> mVecEmpty;
signals:
    void closed();
    void signalActivate();
    void signalSave();
public slots:
    void slotShow(int n);
    void slotClose();
    void slotSaveAndClose();
    void slotActivate();
    void slotSaveThis();
    void slotSave();
};

#endif // STUDENTWIDGET_H

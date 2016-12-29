#ifndef SETTINGWIDGET_H
#define SETTINGWIDGET_H

#include <QMainWindow>
#include <QFile>
#include "layouttoform.h"
#include "tabledisciplinewidget.h"
#include "settingclass.h"
#include "disciplinewidget.h"

class SettingWidget : public QMainWindow
{
    Q_OBJECT
public:
    explicit SettingWidget(SettingClass* st, QWidget *parent = 0);
    ~SettingWidget();
    FormPlusData* form;
protected:
    TableDisciplineWidget* tab;
    SettingClass* setting;
    QString fileName;

    void setActMenu();
    QAction* actSave;
    QAction* actSaveAs;
    QAction* actClose;

    QAction* actAddDiscipline;
    QAction* actRemoveDiscipline;
signals:

public slots:
    void slotSave();
    void slotSaveAs();

    void slotAddDiscipline();
    void slotRemoveDiscipline();
};

#endif // SETTINGWIDGET_H

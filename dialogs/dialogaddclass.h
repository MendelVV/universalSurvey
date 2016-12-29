#ifndef DIALOGADDCLASS_H
#define DIALOGADDCLASS_H

#include <QDialog>
#include "layouttoform.h"
#include "settingclass.h"

class DialogAddClass : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddClass(QString typeGr,SettingClass* setc, QWidget *parent = 0);
    ~DialogAddClass();
    FormPlusData* form;
protected:
    SettingClass* setting;
signals:

public slots:
    void slotAdd();
};

#endif // DIALOGADDCLASS_H

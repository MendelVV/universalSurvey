#ifndef DIALOGADDSTUDENT_H
#define DIALOGADDSTUDENT_H

#include <QDialog>
#include "layouttoform.h"
#include "settingclass.h"

class DialogAddStudent : public QDialog
{
    Q_OBJECT
public:
    explicit DialogAddStudent(QString typePl, SettingClass* setc, QWidget *parent = 0);
    ~DialogAddStudent();
    FormPlusData* form;
protected:
    SettingClass* setting;
signals:

public slots:
    void slotAdd();
};

#endif // DIALOGADDSTUDENT_H

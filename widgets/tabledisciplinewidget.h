#ifndef TABLEDISCIPLINEWIDGET_H
#define TABLEDISCIPLINEWIDGET_H

#include <QtWidgets>
#include "settingclass.h"
#include "disciplinewidget.h"

class TableDisciplineWidget : public QTableWidget
{
    Q_OBJECT
public:
    TableDisciplineWidget(QWidget *parent=0);
    TableDisciplineWidget(SettingClass* st, QWidget *parent=0);

    void addDiscipline(DisciplineClass* dsp);//сама дисциплина
    void removeDiscipline(QString nm);//системное имя
    void reprint();

protected:
    SettingClass* setting;
    QMenu* contMenu;
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:

public slots:
    void slotRefresh();
    void slotRemove();
};

#endif // TABLEDISCIPLINEWIDGET_H

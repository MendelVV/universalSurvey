#ifndef TABLECLASSWIDGET_H
#define TABLECLASSWIDGET_H

#include <QtWidgets>
#include "schoolclass.h"
#include "classwidget.h"

class TableClassWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableClassWidget(SchoolClass* school, QWidget *parent = 0);
    SchoolClass* sch;
    void refresh();
protected:
    void addRow(ClassClass* cls);
    void reprint();

    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
signals:
    void signalOpenClass(ClassClass*);
public slots:
    void slotAddRow(ClassClass*, int);
};

#endif // TABLECLASSWIDGET_H

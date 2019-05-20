#ifndef TABLESTUDENTWIDGET_H
#define TABLESTUDENTWIDGET_H

#include <QtWidgets>
#include "dialogs/dialogaddstudent.h"
#include "classclass.h"

class TableStudentWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit TableStudentWidget(ClassClass* classC, QWidget *parent = 0);
    ClassClass* cls;
    void refresh();
protected:
    QMenu* contMenu;
    void addRow(StudentClass* stud);
    void reprint();
    void mousePressEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *ce);
private:
    QString doubleToPercent(double value);
signals:
    void signalOpenStudent(StudentClass*);
public slots:
    void slotAddRow(StudentClass*, int);
    void slotEdit();
};

#endif // TABLESTUDENTWIDGET_H

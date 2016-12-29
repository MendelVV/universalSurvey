#ifndef MINIVIEW_H
#define MINIVIEW_H

#include <QtWidgets>
#include <QFont>
#include <QPalette>
#include "schoolclass.h"

class MiniView : public QWidget
{
    Q_OBJECT
public:
    MiniView(SchoolClass* school, QWidget *parent = 0);
    MiniView(ClassClass* classC, QWidget *parent = 0);
    MiniView(StudentClass* student, QWidget *parent = 0);
protected:
    SchoolClass* sch;
    ClassClass* cls;
    StudentClass* stud;
    QLabel* lblName;
    QLabel* lblType;
    QLabel* lblLeft;
    void mousePressEvent(QMouseEvent *);
signals:

public slots:

};

#endif // MINIVIEW_H

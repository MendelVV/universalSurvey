#include "miniview.h"

MiniView::MiniView(SchoolClass* school, QWidget *parent) :
    QWidget(parent)
{
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    sch=school;
    stud=0;
    cls=0;
    QFont font("Times",7,QFont::Normal);
    if (sch->getNumber()!=""){
        lblName = new QLabel(sch->getNumber());
    }else{
        lblName = new QLabel("New");
    }

    lblName->setMaximumSize(5*x,2*y);
    lblName->setFont(font);
    QString typeOrg = sch->getSetting()->getTypeOrg().split(";")[2].trimmed().toLower();
    typeOrg = sch->getSetting()->toFirstUpper(typeOrg);
    lblType = new QLabel(typeOrg);
    lblType->setMaximumSize(5*x,2*y);
    lblType->setFont(font);

    QGridLayout* layout = new QGridLayout;
    layout->setHorizontalSpacing(0);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,0);
    layout->setVerticalSpacing(0);
    layout->addWidget(lblName,0,0,Qt::AlignLeft);
    layout->addWidget(lblType,1,0,Qt::AlignLeft);
    setLayout(layout);

    setFixedSize(5*x,5*y);

}

MiniView::MiniView(ClassClass* classC, QWidget *parent) :
    QWidget(parent)
{
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    cls=classC;
    sch=0;
    stud=0;
    QFont font("Times",7,QFont::Normal);
    if (cls->getNumber()!=""){
        lblName = new QLabel(cls->getNumber());
    }else{
        lblName = new QLabel("New");
    }

    lblName->setMaximumSize(5*x,2*y);
    lblName->setFont(font);

    QString typeGr = cls->getSetting()->getTypeGr().split(";")[0].trimmed().toLower();
    typeGr = cls->getSetting()->toFirstUpper(typeGr);
    lblType = new QLabel(typeGr);
    lblType->setMaximumSize(5*x,2*y);
    lblType->setFont(font);

    QGridLayout* layout = new QGridLayout;
    layout->setHorizontalSpacing(0);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,0);
    layout->setVerticalSpacing(0);
    layout->addWidget(lblName,0,0,Qt::AlignLeft);
    layout->addWidget(lblType,1,0,Qt::AlignLeft);
    setLayout(layout);

    setFixedSize(5*x,5*y);

}

MiniView::MiniView(StudentClass* student, QWidget *parent) :
    QWidget(parent)
{
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    stud=student;
    sch=0;
    cls=0;
    QFont font("Times",7,QFont::Normal);
    if (stud->getFio()!=""){
        lblName = new QLabel(stud->getFio());
    }else{
        lblName = new QLabel("New");
    }

    lblName->setMaximumSize(5*x,2*y);
    lblName->setFont(font);
    QString typePl = stud->getSetting()->getTypePl().split(";")[2].trimmed().toLower();
    typePl = stud->getSetting()->toFirstUpper(typePl);
    lblType = new QLabel(typePl);
    lblType->setMaximumSize(5*x,2*y);
    lblType->setFont(font);

    QGridLayout* layout = new QGridLayout;
    layout->setHorizontalSpacing(0);
    layout->setColumnStretch(0,0);
    layout->setColumnStretch(1,0);
    layout->setVerticalSpacing(0);
    layout->addWidget(lblName,0,0,Qt::AlignLeft);
    layout->addWidget(lblType,1,0,Qt::AlignLeft);
    setLayout(layout);

    setFixedSize(5*x,5*y);

}

void MiniView::mousePressEvent(QMouseEvent *event){
    if (event->buttons()==1){
        if (sch!=0){
            sch->signalActiveView();
        }else if (cls!=0){
            cls->signalActiveView();
        }else if (stud!=0){
            stud->signalActiveView();
        }
    }
    QWidget::mousePressEvent(event);
}

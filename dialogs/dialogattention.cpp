#include "dialogattention.h"

DialogAttention::DialogAttention(QWidget *parent) :
    QDialog(parent)
{
    setModal(true);

    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;

    QFont font("Times",11,QFont::Normal);
    QString link ="https://java.com/ru/download/";
    QTextBrowser* brouser = new QTextBrowser;

    brouser->setFont(font);
    brouser->setWindowTitle("Внимание!");
    QString str = "<html><head><title></title></head><body><div><center><font color='black' size='4'>Внимание!</font></center>";
    str = str+"<p align='justify'><font color='black'>Для формирование диаграм и графиков используется java "
                  "если она не установлена на компьютере то графики не будут построены.";
    str = str+"<br><br>Скачать java можно с официального сайта <a href='"+link+"'>java.com</a>.";
    str=str+"</div></body></html>";
    brouser->setHtml(str);
    brouser->setOpenExternalLinks(true);

//    lbl->setAlignment(Qt::AlignJustify);
    brouser->setFixedWidth(40*x);
    brouser->resize(40*x,30*y);
    brouser->show();
    QGridLayout* qgl = new QGridLayout;
    qgl->addWidget(brouser,0,0,1,2);
    QPushButton* butCancel = new QPushButton("Oтмена");
    connect(butCancel,SIGNAL(clicked(bool)),this,SLOT(reject()));
    qgl->addWidget(butCancel,1,0);
    QPushButton* butOk = new QPushButton("Продолжить");
    connect(butOk,SIGNAL(clicked(bool)),this,SLOT(accept()));
    qgl->addWidget(butOk,1,1);
    setLayout(qgl);



}

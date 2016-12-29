#include "dialogyousure.h"

DialogYouSure::DialogYouSure(QString str, QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Удаление");

    QLabel* lbl = new QLabel("Вы уверены, что хотите удалить "+str+"?");

    LayoutToForm* ltf = new LayoutToForm;
    ltf->addWidget(lbl,0,0,1,2);

    QPushButton* pbtOk = new QPushButton("Ok");
    connect(pbtOk,SIGNAL(clicked()),this,SLOT(accept()));
    ltf->addWidget(pbtOk,1,0);

    QPushButton* pbtCancel = new QPushButton("Отмена");
    connect(pbtCancel,SIGNAL(clicked()),this,SLOT(reject()));
    ltf->addWidget(pbtCancel,1,1);
    setLayout(ltf);
}

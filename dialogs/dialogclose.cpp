#include "dialogclose.h"

DialogClose::DialogClose(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    setWindowTitle("Закрыть");
    mSave = false;
    QLabel* lbl = new QLabel("Вы уверены, что хотите выйти?\nНесохраненные данные будут потеряны.");

    QGridLayout* ltf = new QGridLayout;
    ltf->addWidget(lbl,0,0,1,3);

    QPushButton* pbtOk = new QPushButton("Да");
    connect(pbtOk,SIGNAL(clicked()),this,SLOT(accept()));
    ltf->addWidget(pbtOk,1,0);

    QPushButton* pbtSave = new QPushButton("Сохранить");
    connect(pbtSave,SIGNAL(clicked()),this,SLOT(slotSave()));
    ltf->addWidget(pbtSave,1,1);

    QPushButton* pbtCancel = new QPushButton("Отмена");
    connect(pbtCancel,SIGNAL(clicked()),this,SLOT(reject()));
    ltf->addWidget(pbtCancel,1,2);
    setLayout(ltf);

}

void DialogClose::slotSave(){
    mSave = true;
    accept();
}

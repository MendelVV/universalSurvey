#include "dialogaddclass.h"

DialogAddClass::DialogAddClass( QString typeGr,SettingClass* setc, QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setting = setc;
    setWindowTitle("Добавление "+typeGr);
    form = new FormPlusData;
    CellPlusClass* cell;
    cell = new CellPlusClass("lblNumber","Label","Название "+typeGr,0,0,10);
    form->addCell(cell);
    cell = new CellPlusClass("txtNumber","Text","",0,1,10);
    form->addCell(cell);
    cell = new CellPlusClass("lblCode","Label","Код "+typeGr,1,0,10);
    form->addCell(cell);
    cell = new CellPlusClass("txtCode","NumberInt","",1,1,10);
    form->addCell(cell);

    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);

    QPushButton* pbtOk = new QPushButton("Ok");
    connect(pbtOk,SIGNAL(clicked()),this,SLOT(slotAdd()));
    ltf->addWidget(pbtOk,2,0);

    QPushButton* pbtCancel = new QPushButton("Отмена");
    connect(pbtCancel,SIGNAL(clicked()),this,SLOT(reject()));
    ltf->addWidget(pbtCancel,2,1);
    setLayout(ltf);

}

DialogAddClass::~DialogAddClass(){
    delete form;
}

void DialogAddClass::slotAdd(){
    QString code = form->getCell("txtCode")->getValue();
    if (form->getCell("txtNumber")->getValue()!="" && code!=""){
        int sz = setting->getOtherSetting("szgrpcode").toInt();
        if (sz>0 && code.length()!=sz){
            QMessageBox* msb = new QMessageBox(this);
            msb->setWindowTitle("Ошибка");
            QString typeGr = setting->getTypeGr().split(";")[1];
            msb->setText("Количество символов в коде "+typeGr+" должно равняться "+setting->getOtherSetting("szgrpcode")+"!");
            msb->show();
            return;
        }
        accept();
    }else{
        QMessageBox* msb = new QMessageBox(this);
        msb->setWindowTitle("Ошибка");
        msb->setText("Не указан номер или код!");
        msb->show();
    }
}

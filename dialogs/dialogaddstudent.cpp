#include "dialogaddstudent.h"

DialogAddStudent::DialogAddStudent(QString typePl, SettingClass* setc, QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setting = setc;
    setWindowTitle("Добавление "+typePl);
    form = new FormPlusData;
    CellPlusClass* cell;

    cell = new CellPlusClass("lblFio", "Label", "Фио "+typePl,0,0,15);
    form->addCell(cell);
    cell = new CellPlusClass("txtFio", "Text", "",0,1,15);
    form->addCell(cell);
    //нужны настройки
    cell = new CellPlusClass("lblCode", "Label", setting->getOtherSetting("stdcodetitle"),1,0,15);
    form->addCell(cell);
    cell = new CellPlusClass("txtCode", "NumberInt", "",1,1,15);
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

DialogAddStudent::~DialogAddStudent(){
    delete form;
}

void DialogAddStudent::slotAdd(){
    QString code = form->getCell("txtCode")->getValue();
    if (form->getCell("txtFio")->getValue()!="" && code!=""){
        int sz = setting->getOtherSetting("szstdcode").toInt();
        if (sz>0 && code.length()!=sz){
            QMessageBox* msb = new QMessageBox(this);
            msb->setWindowTitle("Ошибка");
            QString typePl = setting->getTypePl().split(";")[0];
            msb->setText("Количество символов в коде "+typePl+" должно равняться "+setting->getOtherSetting("szstdcode")+"!");
            msb->show();
            return;
        }
        accept();
    }else{
        QMessageBox* msb = new QMessageBox(this);
        msb->setWindowTitle("Ошибка");
        msb->setText("Не указано ФИО или код!");
        msb->show();
    }
}

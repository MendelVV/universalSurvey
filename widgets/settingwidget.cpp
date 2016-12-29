#include "settingwidget.h"

SettingWidget::SettingWidget(SettingClass* st, QWidget *parent) : QMainWindow(parent)
{
    setting =st;
    fileName="";
//рисуем форму
    form = new FormPlusData;
    CellPlusClass* cell;
    cell = new CellPlusClass("lblYear", "Label", "Учебный год", 1, 0, 10, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldYear", "Text", st->getYear(), 1, 1, 10, 4);
    form->addCell(cell);

    cell = new CellPlusClass("lblType", "Label", "Тип анкетирования", 1, 2, 10, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldType", "Combobox", st->getType(), 1, 3, 10, 4);
    cell->setList(QStringList()<<"Вход"<<"Выход");
    form->addCell(cell);

    cell = new CellPlusClass("lblMain", "MultyLabel", "НАСТРОЙКИ АНКЕТИРОВАНИЯ", 0, 0, 42, 4);
    cell->setMergeColumn(4);
    form->addCell(cell);

    cell = new CellPlusClass("lblTittle", "Label", "Название", 2, 0, 10, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldTittle", "MultyText", st->getTittle(), 2, 1, 31, 4);
    cell->setMergeColumn(3);
    form->addCell(cell);

    cell = new CellPlusClass("lblTypeOrg", "Label", "организация;организации;орг.", 3, 0, 20, 4);
    cell->setMergeColumn(2);
    form->addCell(cell);

    cell = new CellPlusClass("fldTypeOrg", "Text", st->getTypeOrg(), 3, 2, 20, 4);
    cell->setMergeColumn(2);
    form->addCell(cell);

    cell = new CellPlusClass("lblTypeGr", "Label", "группа;группы;группу;групп", 4, 0, 20, 4);
    cell->setMergeColumn(2);
    form->addCell(cell);

    cell = new CellPlusClass("fldTypeGr", "Text", st->getTypeGr(), 4, 2, 20, 4);
    cell->setMergeColumn(2);
    form->addCell(cell);

    cell = new CellPlusClass("lblTypePl", "Label", "анкетируемого;анкетируемых;анкет.", 5, 0, 20, 4);
    cell->setMergeColumn(2);
    form->addCell(cell);

    cell = new CellPlusClass("fldTypePl", "Text", st->getTypePl(), 5, 2, 20, 4);
    cell->setMergeColumn(2);
    form->addCell(cell);
//добавляем таблицу дисциплин/анкет
    tab = new TableDisciplineWidget(setting);
//========================================
    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);
    ltf->addWidget(tab, 6,0,1,4);
    QWidget* wgt = new QWidget;
    wgt->setLayout(ltf);
    QScrollArea* sc = new QScrollArea;
    sc->setWidget(wgt);
    setCentralWidget(sc);

    setActMenu();
    QMenu* menu = new QMenu("Меню");
    menu->addAction(actSave);
    menu->addAction(actSaveAs);
    menu->addAction(actClose);
    menuBar()->addMenu(menu);

    menu = new QMenu("Дисциплины");
    menu->addAction(actAddDiscipline);
    menu->addAction(actRemoveDiscipline);
    menuBar()->addMenu(menu);
}

SettingWidget::~SettingWidget(){
    delete form;
}

void SettingWidget::setActMenu(){
    actSave = new QAction("Сохранить",0);
    connect(actSave,SIGNAL(triggered(bool)),this,SLOT(slotSave()));

    actSaveAs = new QAction("Сохранить как",0);
    connect(actSaveAs,SIGNAL(triggered(bool)),this,SLOT(slotSaveAs()));

    actClose = new QAction("Закрыть",0);
    connect(actClose,SIGNAL(triggered(bool)),this,SLOT(close()));

    actAddDiscipline = new QAction("Добавить",0);
    connect(actAddDiscipline,SIGNAL(triggered(bool)),this,SLOT(slotAddDiscipline()));

    actRemoveDiscipline = new QAction("Удалить",0);
    connect(actRemoveDiscipline,SIGNAL(triggered(bool)),this,SLOT(slotRemoveDiscipline()));
}

void SettingWidget::slotSave(){
//перед тем как сохранять объекты в файл, записываем данные из формы в объект
    setting->setYear(form->getValue("fldYear"));
    setting->setType(form->getValue("fldType"));
    setting->setTittle(form->getValue("fldTittle"));
    setting->setTypeOrg(form->getValue("fldTypeOrg"));
    setting->setTypeGr(form->getValue("fldTypeGr"));
    setting->setTypePl(form->getValue("fldTypePl"));
//здесь сохраняем в файл ключей (keys.data в папке приложения)
    if (setting->getYear()!="" && setting->getTittle()!=""){
        QString str=QCoreApplication::applicationDirPath()+"/setting.data";
        if (fileName!=""){
            str=fileName;
        }
        setting->save(str);
    }else{
        QMessageBox* msb = new QMessageBox();
        msb->setWindowTitle("Ошибка");
        msb->setText("Файл не может быть сохранен. Не указаны год и название анкетирования.");
        msb->show();
    }

}

void SettingWidget::slotSaveAs(){
//перед тем как сохранять объекты в файл, записываем данные из формы в объект
    setting->setYear(form->getValue("fldYear"));
    setting->setType(form->getValue("fldType"));
    setting->setTittle(form->getValue("fldTittle"));
    setting->setTypeOrg(form->getValue("fldTypeOrg"));
    setting->setTypeGr(form->getValue("fldTypeGr"));
    setting->setTypePl(form->getValue("fldTypePl"));
//здесь сохраняем в файл ключей (keys.data в папке приложения)
    if (setting->getYear()!="" && setting->getTittle()!=""){
        QString str = QFileDialog::getExistingDirectory(this, "Сохранение");
        if (str!=""){
            setting->save(str);
        }
    }else{
        QMessageBox* msb = new QMessageBox();
        msb->setWindowTitle("Ошибка");
        msb->setText("Файл не может быть сохранен. Не указаны год и название анкетирования.");
        msb->show();
    }

}

void SettingWidget::slotAddDiscipline(){
    DisciplineWidget* dspw = new DisciplineWidget;
    if (dspw->exec()==QDialog::Accepted){
        if (dspw->form->getValue("fldNameSystem")!="" && dspw->form->getValue("fldNameDiscipline")!=""){
            DisciplineClass* dsp = new DisciplineClass();
            dsp->setNameSystem(dspw->form->getValue("fldNameSystem"));
            dsp->setNameDiscipline(dspw->form->getValue("fldNameDiscipline"));
            dsp->setType(dspw->form->getValue("fldType"));
            dsp->setCount(dspw->form->getValue("fldCount").toInt());
            setting->addDiscipline(dsp);
        }else{
            QMessageBox* msb = new QMessageBox();
            msb->setWindowTitle("Ошибка");
            msb->setText("Анкета/Дисциплина не может быть добавлена. Не хватает названия.");
            msb->show();
        }
    }
}

void SettingWidget::slotRemoveDiscipline(){
    tab->slotRemove();
}

#include "schoolwidget.h"

SchoolWidget::SchoolWidget(SchoolClass* school, QWidget *parent) :
    QMainWindow(parent)
{
    sch = school;
    connect(sch,SIGNAL(signalActiveView()),this,SLOT(slotActivate()));
    connect(sch,SIGNAL(signalResizeStudent()),this,SLOT(slotRefresh()));
    fileName = "";

    form = new FormPlusData;
    CellPlusClass* cell;
    QFont font("Times",14,QFont::Normal);
    QString typeOrg = sch->getSetting()->getTypeOrg().split(";")[0].trimmed().toLower();
    typeOrg = sch->getSetting()->toFirstUpper(typeOrg);
    QLabel* lblTitle = new QLabel(typeOrg);
    lblTitle->setFont(font);

    typeOrg = sch->getSetting()->getTypeOrg().split(";")[1].trimmed().toLower();
    cell = new CellPlusClass("lblSchoolNumber", "Label", "Название "+typeOrg, 0,0,11);
    form->addCell(cell);
    cell = new CellPlusClass("txtSchoolNumber", "Text", school->getNumber(), 0,1,11);
    form->addCell(cell);
    cell = new CellPlusClass("lblSchoolCode", "Label", "Код "+typeOrg, 0,2,11);
    form->addCell(cell);
    cell = new CellPlusClass("txtSchoolCode", "NumberInt", school->getCode(), 0,3,11);
    form->addCell(cell);
    QString typeGr = sch->getSetting()->getTypeGr().split(";")[3].trimmed().toLower();
    cell = new CellPlusClass("lblClassList", "Label", "Список "+typeGr, 1,0,22);
    cell->setMergeColumn(2);
    form->addCell(cell);


    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);
    tcw = new TableClassWidget(sch);
    connect(tcw, SIGNAL(signalOpenClass(ClassClass*)),this,SLOT(slotOpenClass(ClassClass*)));
    ltf->addWidget(tcw,2,0,1,4,Qt::AlignLeft);

    QPushButton* pbtAdd = new QPushButton("Добавить");
    connect(pbtAdd,SIGNAL(clicked()),this,SLOT(slotAddClass()));
    ltf->addWidget(pbtAdd,1,2);

    QPushButton* pbtRemove = new QPushButton("Удалить");
    connect(pbtRemove,SIGNAL(clicked()),this,SLOT(slotRemoveClass()));
    ltf->addWidget(pbtRemove,1,3);

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(lblTitle,0);
    layout->addLayout(ltf,1);
    QWidget* wgt = new QWidget;
    wgt->setLayout(layout);
    QScrollArea* sc = new QScrollArea;
    sc->setWidget(wgt);
    setCentralWidget(sc);

    QMenu* menu = new QMenu("&Меню");
    setMainMenu();
    menu->addAction(actSave);
    menu->addAction(actSaveAs);
    menu->addSeparator();
    menu->addAction(actToXML);
    menu->addSeparator();
    menu->addAction(actExit);
    menuBar()->addMenu(menu);
    menuBar()->addSeparator();

    QMenu* menuProperty = new QMenu("&Свойства");
    setPropertyMenu();
    menuProperty->addAction(actAddClass);
    menuBar()->addMenu(menuProperty);
}

SchoolWidget::~SchoolWidget(){
    delete form;
}

void SchoolWidget::closeEvent(QCloseEvent *ce){
    sch->closedView(sch);
}

void SchoolWidget::setMainMenu(){
    actSave = new QAction("Сохранить",0);
    connect(actSave,SIGNAL(triggered()),this,SLOT(slotSave()));

    actSaveAs = new QAction("Сохранить как",0);
    connect(actSaveAs,SIGNAL(triggered()),this,SLOT(slotSaveAs()));

    actToXML = new QAction("Конвертировать в XML",0);
    connect(actToXML,SIGNAL(triggered()),this,SLOT(slotToXML()));

    actExit = new QAction("Выйти",0);
    connect(actExit,SIGNAL(triggered()),this,SLOT(slotClose()));
}

void SchoolWidget::setPropertyMenu(){
    QString typeGr = sch->getSetting()->getTypeGr().split(";")[2].trimmed().toLower();
    actAddClass = new QAction("Добавить "+typeGr,0);
    connect(actAddClass,SIGNAL(triggered()),this,SLOT(slotAddClass()));
}

void SchoolWidget::slotOpenClass(ClassClass *cls){
    emit signalOpenClass(cls);
}

void SchoolWidget::slotClose(){
    emit closed();
}

void SchoolWidget::slotSave(){
    SettingClass* setting = sch->getSetting();
    QString code =  form->getCell("txtSchoolCode")->getValue();
    if (form->getCell("txtSchoolNumber")->getValue()!="" && code!=""){//если поставлен код и название
        int sz = setting->getOtherSetting("szorgcode").toInt();
        if (sz>0 && code.length()!=sz){
            QMessageBox* msb = new QMessageBox(this);
            msb->setWindowTitle("Ошибка");
            QString typeOrg = setting->getTypeOrg().split(";")[1];
            msb->setText("Количество символов в коде "+typeOrg+" должно равняться "+setting->getOtherSetting("szorgcode")+"!");
            msb->show();
            return;
        }
        if (fileName!=""){
            sch->setCode(form->getCell("txtSchoolCode")->getValue());
            sch->setNumber(form->getCell("txtSchoolNumber")->getValue());
            sch->saveSchoolClass(fileName);
            activateWindow();
        }else{
            slotSaveAs();
        }

    }else{
        QMessageBox* msb = new QMessageBox;
        msb->setWindowTitle("Ошибка");
        QString typeOrg = sch->getSetting()->getTypeOrg().split(";")[1].trimmed().toLower();
        msb->setText("Невозможно сохранить без названия и кода "+typeOrg+".");
        msb->show();
    }
}

void SchoolWidget::slotSaveAs(){
    SettingClass* setting = sch->getSetting();
    if (form->getCell("txtSchoolNumber")->getValue()!="" && form->getCell("txtSchoolNumber")->getValue()!=""){//если поставлен код и название
 //       QFileDialog* openD = new QFileDialog(this);
        QString str = QFileDialog::getExistingDirectory(this, "Выберите дирректорию для сохранения");
        bool b = true;
        if (str!=""){
            QString code = form->getCell("txtSchoolCode")->getValue();
            int sz = setting->getOtherSetting("szorgcode").toInt();
            if (sz>0 && code.length()!=sz){
                QMessageBox* msb = new QMessageBox(this);
                msb->setWindowTitle("Ошибка");
                QString typeOrg = setting->getTypeOrg().split(";")[1];
                msb->setText("Количество символов в коде "+typeOrg+" должно равняться "+setting->getOtherSetting("szorgcode")+"!");
                msb->show();
                return;
            }

            sch->setCode(code);
            sch->setNumber(form->getCell("txtSchoolNumber")->getValue());
            str=str+"/"+sch->getNumber()+"_"+sch->getCode();
            QVector<ClassClass*> vecCls = sch->getAllClasses();
            int n = vecCls.count();
            for (int i=0; i<n;i++){
                ClassClass* cls = vecCls[i];
                if (cls->getCode()!=""){
                    str=str+"_"+cls->getCode();
                }else{
                    b=false;
                }
            }
            str=str+".scpd";//system for collection personal data
        }
        if (b){
            if (sch->saveSchoolClass(str)){
                fileName=str;
                activateWindow();
            }else{
                QString typeOrg = sch->getSetting()->getTypeOrg().split(";")[1].trimmed().toLower();
                QMessageBox* msb = new QMessageBox;
                msb->setWindowTitle("Ошибка");
                msb->setText("Невозможно сохранить файл! Название "+typeOrg+" содержит недопустимые символы - \\/\"<>:; или др.!");
                msb->show();
            }
        }else{
            QString typeGr = sch->getSetting()->getTypeGr().split(";")[3].trimmed().toLower();
            QMessageBox* msb = new QMessageBox;
            msb->setWindowTitle("Ошибка");
            msb->setText("Невозможно сохранить без указания всех кодов "+typeGr+".");
            msb->show();
        }
    }else{
        QMessageBox* msb = new QMessageBox;
        msb->setWindowTitle("Ошибка");
        msb->setText("Невозможно сохранить без названия и кода организации.");
        msb->show();
    }
}

void SchoolWidget::slotToXML(){
    if (form->getCell("txtSchoolNumber")->getValue()!="" && form->getCell("txtSchoolNumber")->getValue()!=""){//если поставлен код и название
        QString str = QFileDialog::getExistingDirectory(this, "Выберите дирректорию для сохранения");
        if (str!=""){
            sch->setCode(form->getCell("txtSchoolCode")->getValue());
            sch->setNumber(form->getCell("txtSchoolNumber")->getValue());
            str=str+"/"+sch->getNumber()+"_"+sch->getCode()+".xml";
            sch->saveSchoolXML(str);

            activateWindow();
        }
    }else{
        QMessageBox* msb = new QMessageBox;
        msb->setWindowTitle("Ошибка");
        QString typeOrg = sch->getSetting()->getTypeOrg().split(";")[1].trimmed().toLower();
        msb->setText("Невозможно сохранить без названия и кода "+typeOrg+".");
        msb->show();
    }
}

void SchoolWidget::slotRefresh(){
    tcw->refresh();
}

void SchoolWidget::slotAddClass(){
    QString typeGr = sch->getSetting()->getTypeGr().split(";")[1].trimmed().toLower();
    DialogAddClass* dlg = new DialogAddClass(typeGr,sch->getSetting());
    if (dlg->exec()==QDialog::Accepted){
        ClassClass* cls = new ClassClass(sch->getSetting());
        cls->setCode(dlg->form->getCell("txtCode")->getValue());
        cls->setNumber(dlg->form->getCell("txtNumber")->getValue());
        sch->addClass(cls);//добавили класс к школе
        //нужно теперь добавить строку в таблицу
    }
//    ClassClass* cls = new ClassClass;
//    emit signalOpenClass(cls);
}

void SchoolWidget::slotRemoveClass(){
    QTableWidgetItem* item = tcw->item(tcw->currentRow(), 0);
    if (item!=0){
        QString typeGr = sch->getSetting()->getTypeGr().split(";")[1].trimmed().toLower();
        DialogYouSure* dlg = new DialogYouSure(typeGr);
        if (dlg->exec()==QDialog::Accepted){
            sch->removeClass(item->text());
            tcw->refresh();
        }
    }
}

void SchoolWidget::slotActivate(){
    emit signalActivate();
}

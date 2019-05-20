#include "studentwidget.h"

StudentWidget::StudentWidget(StudentClass* student, QWidget *parent) :
    QMainWindow(parent)
{
    stud = student;
    connect(stud,SIGNAL(signalActiveView()),this,SLOT(slotActivate()));
    connect(stud,SIGNAL(signalSaveView()),this,SLOT(slotSave()));
    setForms();

    setActMenu();
    QMenu* menu = new QMenu("&Меню");
    menu->addAction(actSave);
    menu->addSeparator();
    menu->addAction(actSaveAndClose);
    menu->addAction(actClose);
    menuBar()->addMenu(menu);
}

StudentWidget::~StudentWidget(){
    QList<QString> lst = mapForm.uniqueKeys();
    int n = lst.count();
    for (int i=0;i<n;i++){
        delete mapForm[lst[i]];
    }
    n = mVecEmpty.size();
    for (int i=0;i<n;i++){
        delete mVecEmpty[i];
    }
}

//=================================================================================================
void StudentWidget::setForms(){
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    mTab = new QTabWidget;
    SettingClass* setting = stud->getSetting();
    qDebug()<<"create tab";
    QMap<QString, QString> mapMaked = stud->getMaked();
//=============================================================
    QMap<QString, DisciplineClass*> mapM =stud->getSetting()->getDisciplines("MoreData");
    QList<QString> lstM=mapM.uniqueKeys();
    for (int i=0;i<lstM.count();i++){
        if (mapMaked[lstM[i]]=="1"){//если он писал эту анкету
            form = new FormPlusData;
            if (stud->getSetting()->getType()=="Вход"){
                form->readFormPlusData(QCoreApplication::applicationDirPath()+"/enter/Form"+lstM[i]+".cfpoq");
            }else if (stud->getSetting()->getType()=="Выход"){
                form->readFormPlusData(QCoreApplication::applicationDirPath()+"/exit/Form"+lstM[i]+".cfpoq");
            }else{
                form->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/Form"+lstM[i]+".cfpoq");
            }
            mapForm[lstM[i]]=form;
            setForm(lstM[i]);//вписывем данные
            QWidget* wgt = new QWidget();
            mVecEmpty<<wgt;
            int pos = mTab->addTab(wgt,mapM[lstM[i]]->getNameDiscipline());
            mMapWgtPos[lstM[i]]=pos;
/*            LayoutToForm* ltf = new LayoutToForm;
            ltf->addFormData(mapForm[lstM[i]]);
            ScrollWidget* sc = new ScrollWidget(ltf);
            tab->addTab(sc,mapM[lstM[i]]->getNameDiscipline());*/
        }
    }
//============================================

//    QMap<QString, DisciplineClass*> mapD = stud->getSetting()->getDisciplines("Person");
//    QList<QString> lst=mapD.uniqueKeys();

    QVector<DisciplineClass*> lst = stud->getSetting()->getVecDisciplines("Person");
    for (int i=0;i<lst.count();i++){
        qDebug()<<lst[i]->getNameDiscipline();
        DisciplineClass* dsp = lst[i];
//    foreach (DisciplineClass* dsp, lst){
//        if (mapMaked[lst[i]]=="1"){//если он писал эту анкету
//            QString formName = lst[i];
        QString dspName =  setting->toFirstUpper(dsp->getNameSystem());
        if (mapMaked[dspName]=="1"){
            form = new FormPlusData;
            if (stud->getSetting()->getType()=="Вход"){
                form->readFormPlusData(QCoreApplication::applicationDirPath()+"/enter/Form"+dspName+".cfpoq");
            }else if (stud->getSetting()->getType()=="Выход"){
                form->readFormPlusData(QCoreApplication::applicationDirPath()+"/exit/Form"+dspName+".cfpoq");
            }else{
                form->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/Form"+dspName+".cfpoq");
            }

//            mapForm[lst[i]]=form;
            mapForm[dspName]=form;
            //setForm(lst[i]);//вписывем данные
            setForm(dspName);//вписывем данные
            QWidget* wgt = new QWidget();
            mVecEmpty<<wgt;
//            int pos = mTab->addTab(wgt,mapD[lst[i]]->getNameDiscipline());
            int pos = mTab->addTab(wgt,dsp->getNameDiscipline());
            //mMapWgtPos[lst[i]]=pos;
            mMapWgtPos[dspName]=pos;
/*            LayoutToForm* ltf = new LayoutToForm;
            ltf->addFormData(mapForm[lst[i]]);
            ScrollWidget* sc = new ScrollWidget(ltf);
            tab->addTab(sc,mapD[lst[i]]->getNameDiscipline());*/
        }
    }


    mTab->setFixedSize(88*x,79*y);//изменил

    QFont font("Times",14,QFont::Normal);
    QLabel* lblTitle = new QLabel(stud->getFio());
    lblTitle->setFont(font);
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(lblTitle,0);
    layout->addWidget(mTab,1);

    QWidget* wgt = new QWidget;
    wgt->setLayout(layout);
    QScrollArea* sc = new QScrollArea;
    sc->setWidget(wgt);
    setCentralWidget(sc);
    slotShow(0);
    connect(mTab,SIGNAL(currentChanged(int)),this,SLOT(slotShow(int)));
}
//===================================================================================

void StudentWidget::closeEvent(QCloseEvent *ce){
    stud->closedView(stud);
}

void StudentWidget::setActMenu(){
    actSave = new QAction("Сохранить",0);
    connect(actSave, SIGNAL(triggered()),this,SLOT(slotSaveThis()));

    actClose = new QAction("Закрыть без сохранения",0);
    connect(actClose,SIGNAL(triggered()),this,SLOT(slotClose()));

    actSaveAndClose = new QAction("Сохранить и закрыть",0);
    connect(actSaveAndClose,SIGNAL(triggered()),this,SLOT(slotSaveAndClose()));
}

void StudentWidget::setForm(QString nm){
    int n = stud->balls[nm]->getSize();
    for (int i=0;i<n;i++){//пошли по всем полям
        QString str = "fldQ_"+QString::number(i+1);//берем название каждого последующего вопроса
        if (mapForm[nm]->contains(str)){//если есть такая ячейка
            CellPlusClass* cell = mapForm[nm]->getCell(str);
            cell->setValue(stud->balls[nm]->getBall(i));//то поставили значение в соответствующее поле
         }else{//если такого поля нет. Действуем следующим образом:
            QString val=stud->balls[nm]->getBall(i);//значение которое в балле есть
            QStringList lst = val.split(";");
//======================================================
            int j=1;
            QString strC = str+"_"+QString::number(j);
            while (mapForm[nm]->contains(strC)){
                CellPlusClass* cell = mapForm[nm]->getCell(strC);
                if (cell->getTypeCell()=="Checkbox"){//в этом случае запоминаем номер
                    //if (lst.contains(QString::number(j))){//если есть номер этой ячейки в баллах
                    if (lst.contains(cell->getFormula())){
                        cell->setValue("1");
                    }
                }else{//в остальных само значение
                    if (lst.count()>=j){
                        cell->setValue(lst[j-1]);
                    }
                }
                j++;
                strC=str+"_"+QString::number(j);
            }
//======================================================
        }
    }
}

void StudentWidget::slotClose(){
    emit closed();
}

void StudentWidget::slotSaveAndClose(){
    mTab->setCurrentIndex(0);//костыль
    slotSaveThis();
    slotClose();
    stud->closedView(stud);
}

void StudentWidget::slotActivate(){
    emit signalActivate();
}

void StudentWidget::slotSaveThis(){
    slotSave();
    emit signalSave();

}

void StudentWidget::slotSave(){
//берем все предменты которые заполняются индивидуально
    QList<QString> lst = stud->getSetting()->getDisciplines("Person").uniqueKeys();
    lst<<stud->getSetting()->getDisciplines("MoreData").uniqueKeys();
    for (int i=0;i<lst.count();i++){    
        if (stud->getMaked(lst[i])=="1" && mMapWgt.contains(lst[i])){
            int n = stud->balls[lst[i]]->getSize();//количество вопросов в анкете
            for (int j=0;j<n;j++){
                QString str = "fldQ_"+QString::number(j+1);//берем название каждого последующего вопроса
                if (mapForm[lst[i]]->contains(str)){//если есть такая ячейка
                    CellPlusClass* cell = mapForm[lst[i]]->getCell(str);
                    stud->balls[lst[i]]->setBall(j, cell->getValue());
                }else{//если такого поля нет. Действуем следующим образом:
                    QString val="";//значение которое будем записывать в студента
                    int k=1;
                    QString strC = str+"_"+QString::number(k);
                    while (mapForm[lst[i]]->contains(strC)){
                        CellPlusClass* cell = mapForm[lst[i]]->getCell(strC);
                        if (cell->getTypeCell()=="Checkbox"){//в этом случае запоминаем номер
                            if (cell->getValue()=="1"){
                                if (val.length()==0){
                                //    val=QString::number(k);
                                    val = cell->getFormula();
                                }else{
                                //    val=val+";"+QString::number(k);
                                    val = val+";"+cell->getFormula();
                                }

                            }
                        }else{//в остальных само значение
                            if (val.length()==0){
                                val=cell->getValue();
                            }else{
                                val=val+";"+cell->getValue();
                            }
                        }
                        k++;
                        strC=str+"_"+QString::number(k);
                    }
                    stud->balls[lst[i]]->setBall(j, val);
                    if (k==1){
                        qWarning()<<"небыло найдено такое поле. Форма"<<lst[i]<<"Поле"<<str;
                    }
                }
            }
        }
    }
}

void StudentWidget::slotShow(int n){
    //нужно знать какую форму вставляем и из какой части
    if (!mMapWgtPos.values().contains(n)) return;
    QString str = mMapWgtPos.key(n);
    if (!mMapWgt.contains(str) && mapForm.contains(str)){
        QString title = mTab->tabText(n);

        LayoutToForm* ltf = new LayoutToForm;
        ltf->addFormData(mapForm[str]);
        ScrollWidget* sc = new ScrollWidget(ltf);
        mTab->insertTab(n,sc,title);
        mMapWgt[str]=sc;
        mTab->setCurrentWidget(sc);
        mTab->removeTab(n+1);
    }
}

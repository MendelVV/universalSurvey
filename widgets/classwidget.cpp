#include "classwidget.h"

ClassWidget::ClassWidget(ClassClass* classC, int md, QWidget *parent) :
    QMainWindow(parent)
{
    mode=md;
    cls = classC;
    connect(cls,SIGNAL(signalActiveView()),this,SLOT(slotActivate()));
    connect(cls,SIGNAL(signalSaveView()),this,SLOT(slotSave()));
    connect(cls,SIGNAL(signalDublicate(QString)),this,SLOT(slotDulicate(QString)));
    y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    x =((double)GetSystemMetrics(SM_CXSCREEN))/100;

    setting = cls->getSetting();
    setActMenu();
    QMenu* menu = new QMenu("&Меню");
    menu->addAction(actSave);
    menu->addSeparator();
    menu->addAction(actAddStudent);
    menu->addSeparator();
    menu->addAction(actClose);
    menuBar()->addMenu(menu);
    menuBar()->addSeparator();

    setEnterForm();
}

ClassWidget::~ClassWidget(){
    delete form;
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

void ClassWidget::setEnterForm(){
    tab = new QTabWidget;
    connect(tab,SIGNAL(currentChanged(int)),this,SLOT(slotShow(int)));
    if (mode==0){
        tab->setFixedSize(88*x,79*y);//изменил
    }

    QFont font("Times",14,QFont::Normal);
    QString typeGr = setting->getTypeGr().split(";")[0].trimmed().toLower();
    typeGr = setting->toFirstUpper(typeGr);
    QLabel* lblTitle = new QLabel(typeGr);
    lblTitle->setFont(font);

    typeGr = setting->getTypeGr().split(";")[1].trimmed().toLower();
    form = new FormPlusData;
    CellPlusClass* cell;
    cell = new CellPlusClass("lblClassNumber", "Label", "Название "+typeGr, 0,0,11);
    form->addCell(cell);
    cell = new CellPlusClass("txtClassNumber", "Text", cls->getNumber(), 0,1,11);
    form->addCell(cell);
    cell = new CellPlusClass("lblClassCode", "Label", "Код "+typeGr, 0,2,11);
    form->addCell(cell);
    cell = new CellPlusClass("txtClassCode", "Text", cls->getCode(), 0,3,11);
    form->addCell(cell);
    cell = new CellPlusClass("lblClassList", "Label", "Список "+typeGr, 1,0,22);
    cell->setMergeColumn(2);
    form->addCell(cell);

    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);

    tsw = new TableStudentWidget(cls);
    ltf->addWidget(tsw, 2,0,1,4);
    connect(tsw,SIGNAL(signalOpenStudent(StudentClass*)),this,SLOT(slotOpenStudent(StudentClass*)));

    QPushButton* pbtAdd = new QPushButton("Добавить");
    connect(pbtAdd,SIGNAL(clicked()),this,SLOT(slotAddStudent()));
    ltf->addWidget(pbtAdd,1,2);

    QPushButton* pbtRemove = new QPushButton("Удалить");
    connect(pbtRemove,SIGNAL(clicked()),this,SLOT(slotRemoveStudent()));
    ltf->addWidget(pbtRemove,1,3);

    QWidget* wgt;
    QScrollArea* sc;
    wgt = new QWidget;
    wgt->setLayout(ltf);
    sc = new QScrollArea;
    sc->setWidget(wgt);

    typeGr = setting->getTypeGr().split(";")[1].trimmed().toLower();
    tab->addTab(sc,"Список "+typeGr);
    QList<QString> lstC = setting->getDisciplines("Classed").uniqueKeys();//список всех анкет для класа
    for (int i=0;i<lstC.count();i++){
//добавляем формы
        setFormClassed(lstC[i]);
    }

    if (cls->hasMainForm){//если нужно добавили главную форму
        setFormMainEnter();
    }

    QList<QString> lst = cls->mapHasForm.uniqueKeys();
    for (int i=0;i<lst.count();i++){//идем по всем возможным активным формам
        if (cls->mapHasForm[lst[i]]){//если форма активна то добавляем ее
            setForm(lst[i]);//lst[i] - это например "PrintMapEnter" без слова "Form"
        }
    }

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(lblTitle,0);
    layout->addWidget(tab,1);

    wgt = new QWidget;
    wgt->setLayout(layout);
    sc = new QScrollArea;
    sc->setWidget(wgt);
    setCentralWidget(sc);

    setActAddForm();

    QMenu* menu = new QMenu("&Добавить форму");
    menu->addAction(actAddMainForm);
    lst.clear();
    QMap<QString, DisciplineClass*> mapD = setting->getDisciplines("Shared");
    lst=mapD.uniqueKeys();
    for (int i=0;i<lst.count();i++){
        mapActAdd[lst[i]]=new QAction(mapD[lst[i]]->getNameDiscipline(),0);
        connect(mapActAdd[lst[i]], SIGNAL(triggered()),this,SLOT(slotAddForm()));
        menu->addAction(mapActAdd[lst[i]]);
    }
    menu->addSeparator();

    menuBar()->addMenu(menu);
    menuBar()->addAction(actRefreshForms);
    if (KeyHelper::readMaybeLevels().size()>0){
        menuBar()->addSeparator();
        menuBar()->addAction(actResults);
    }
    menuBar()->addSeparator();
    menuBar()->addAction(actUpdateCompleted);


}

void ClassWidget::closeEvent(QCloseEvent*){
    cls->closedView(cls);
}

void ClassWidget::setActMenu(){
    actSave = new QAction("Сохранить",0);
    connect(actSave, SIGNAL(triggered()),this,SLOT(slotSaveThis()));
    QString typePl = cls->getSetting()->getTypePl().split(";")[0].trimmed().toLower();
    actAddStudent = new QAction("Добавить "+typePl,0);
    connect(actAddStudent,SIGNAL(triggered()),this,SLOT(slotAddStudent()));

    actClose = new QAction("Сохранить и закрыть",0);
    connect(actClose,SIGNAL(triggered()),this,SLOT(slotSaveAndClose()));

    actResults = new QAction("Результаты",0);
    connect(actResults,SIGNAL(triggered(bool)),this,SLOT(slotResults()));

    actUpdateCompleted = new QAction("Процент заполнения",0);
    connect(actUpdateCompleted,SIGNAL(triggered(bool)),this,SLOT(slotUpdateCompleted()));
}

void ClassWidget::setActAddForm(){
    actAddMainForm = new QAction("Основная",0);
    connect(actAddMainForm, SIGNAL(triggered()),this,SLOT(slotAddMainForm()));

    actRefreshForms = new QAction("Обновить формы",0);
    connect(actRefreshForms, SIGNAL(triggered()),this,SLOT(slotRefreshForms()));
}

void ClassWidget::setFormMainEnter(){
    FormPlusData* formPData =  new FormPlusData;
    formPData->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/FormList.cfpoq");
    mapForm["FormList"]=formPData;
    QMap<QString,DisciplineClass*> mapM = setting->getDisciplines("MoreData");

//==============================================
    QList<QString> lstM=mapM.uniqueKeys();//список форм
    //вставляем ячейки анкет
    for (int i=0;i<lstM.count();i++){
        QString nm = lstM[i].mid(0,1).toUpper()+lstM[i].mid(1);
        CellPlusClass* cell = new CellPlusClass("lbl"+nm, "MultyLabel", mapM[lstM[i]]->getNameDiscipline(), 0, 5+i, 6, 15);
        mapForm["FormList"]->addCell(cell);
        cell = new CellPlusClass("cmb"+nm, "Combobox", "", 1, 5+i, 6, 5);
        cell->setList(QStringList()<<"0"<<"1");
        mapForm["FormList"]->addCell(cell);
    }

//==============================================

    QMap<QString,DisciplineClass*> mapD = setting->getDisciplines("Studented");
    QVector<DisciplineClass*> vecF = setting->getVecDisciplines("Studented");
    QList<QString> lstF;
//    QList<QString> lstF=mapD.uniqueKeys();//список форм
    //вставляем ячейки анкет
    for (int i=0;i<vecF.count();i++){
        lstF<<mapD.key(vecF[i]);//->getNameSystem();
        //QString nm = lstF[i].mid(0,1).toUpper()+lstF[i].mid(1);
        QString nm = setting->toFirstUpper(vecF[i]->getNameSystem());
//        CellPlusClass* cell = new CellPlusClass("lbl"+nm, "MultyLabel", "Выполнение работы \""+mapD[lstF[i]]->getNameDiscipline()+"\"", 0, 5+i+lstM.count(), 6, 15);
        CellPlusClass* cell = new CellPlusClass("lbl"+nm, "MultyLabel", "Выполнение работы \""+vecF[i]->getNameDiscipline()+"\"", 0, 5+i+lstM.count(), 6, 15);
        mapForm["FormList"]->addCell(cell);
        cell = new CellPlusClass("cmb"+nm, "Combobox", "", 1, 5+i+lstM.count(), 6, 5);
        cell->setList(QStringList()<<"0"<<"1");
        mapForm["FormList"]->addCell(cell);
    }
    lstF<<lstM;//добавили в общий список еще и доп сведения
    //===========================
    //узнаем количество школьников в классе и берем их всех
    int n = cls->count;
    QVector<StudentClass*> vec = cls->getAllStudents();
    //проходим все ячейки формы
    QList<CellPlusClass*> cells = mapForm["FormList"]->getList();

    int k = cells.count();//количество ячеек
    int multyRow = 1;//задали множественную строку
    for (int i=0;i<k;i++){//для каждой ячейки
        if (cells[i]->getRow()==multyRow){//если это вторая строка (множественная строка в этой форме вторая)
            mapForm["FormList"]->removeCell(cells[i]);//убрали ячейку из формы
            QString nameCell = cells[i]->getNameCell();
            for (int j=0;j<n;j++){//для каждого ученика класса
                CellPlusClass* cell = cells[i]->copy();//копируем ячейку
                cell->setRow(j+multyRow);//задаем новую строку
                cell->setNameCell(nameCell+"_"+vec[j]->getCode());//задаем новое имя ячейки
                mapForm["FormList"]->addCell(cell);//добавляем ячейку в форму
        //задаем ячейкам соответствующие значения в зависимости от их имени
                if (nameCell=="numNum"){//если это порядеовый номер, то дописываем порядковый номер
                    cell->setValue(QString::number(j+1));
                }else if (nameCell=="txtFIO"){
                    cell->setValue(vec[j]->getFio());
                }else if(nameCell=="txtCode"){
                    cell->setValue(vec[j]->getCode());
                }else if(nameCell=="cmbSex"){
                    cell->setValue(vec[j]->getSex());
                }else if(nameCell=="numDate"){
                    cell->setValue(vec[j]->getBithday());
                }else{
                //здесь можно сделать цикл по всем дисциплинам.

                    for (int jf=0;jf<lstF.count();jf++){
                        if (nameCell=="cmb"+lstF[jf]){
                            cell->setValue(vec[j]->getMaked(lstF[jf]));
                            break;
                        }
                    }
                }

        //=================================================================
            }
        }
    }

    QWidget* wgt = new QWidget;
    mVecEmpty<<wgt;
/*    if (mode==0){
        LayoutToForm* ltf = new LayoutToForm;
        ltf->addFormData(mapForm["FormList"]);
        ScrollWidget* sc = new ScrollWidget(ltf);
        wgt=sc;
    }*/

    int pos = tab->addTab(wgt,"Основные данные");
    mMapWgtPos["FormList"]=pos;
}

void ClassWidget::setForm(QString str){
    FormPlusData* formData = new FormPlusData;
    if (setting->getType()=="Вход"){
        formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/enter/Form"+str+".cfpoq");
    }else if (setting->getType()=="Выход"){
        formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/exit/Form"+str+".cfpoq");
    }else{
        formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/Form"+str+".cfpoq");
    }
    mapForm[str]=formData;
//узнаем количество школьников в классе и берем их всех
    int n = cls->count;
    int pos =0;
    QVector<StudentClass*> vec = cls->getAllStudents();
//проходим все ячейки формы
    QList<CellPlusClass*> cells =  mapForm[str]->getList();
    int k = cells.count();//количество ячеек
    int multyRow = 2;//задали множественную строку
    for (int i=0;i<k;i++){//для каждой ячейки
        if (cells[i]->getRow()==multyRow){//если это вторая строка (множественная строка в этой форме вторая)
            mapForm[str]->removeCell(cells[i]);//убрали ячейку из формы
            QString nameCell = cells[i]->getNameCell();
            pos=0;
            for (int j=0;j<n;j++){//для каждого ученика класса
                //если ученик писал эту работу
                if (vec[j]->getMaked(str)=="1"){

                    CellPlusClass* cell = cells[i]->copy();//копируем ячейку
                    cell->setRow(pos+multyRow);//задаем новую строку
                    cell->setNameCell(nameCell+"_"+vec[j]->getCode());//задаем новое имя ячейки

                    mapForm[str]->addCell(cell);//добавляем ячейку в форму
    //задаем ячейкам соответствующие значения в зависимости от их имени
                    if (nameCell=="numNum"){//если это порядеовый номер, то дописываем порядковый номер
                        cell->setValue(QString::number(pos+1));
                    }else if (nameCell=="txtFIO"){
                        cell->setValue(vec[j]->getFio());
                    }else if(nameCell=="numNumChild"){
                        cell->setValue(vec[j]->getCode());
                    }else if(nameCell=="numVariant"){
                        cell->setValue(vec[j]->balls[str]->getVariant());
                    }else{
                        for (int i=0;i<vec[j]->balls[str]->getSize();i++){
                            if (nameCell=="numNumTask"+QString::number(i+1)){
                                cell->setValue(vec[j]->balls[str]->getBall(i));
                                break;
                            }
                        }
                    }
                    pos++;
                }
//=================================================================
            }
        }
    }

    QWidget* wgt = new QWidget();

//    TableFormWidget* tbForm = new TableFormWidget(mapForm[str]);
//    tbForm->setFixedSize(87*x,75*y);
//    wgt = (QWidget*) tbForm;
    mVecEmpty<<wgt;
    int p = tab->addTab(wgt,setting->getDiscipline(str)->getNameDiscipline());
    mMapWgtPos[str]=p;
/*    if (mode==0){
        LayoutToForm* ltf = new LayoutToForm;
        ltf->addFormData(mapForm[str]);
        wgt = new QWidget;
        wgt->setLayout(ltf);
        QScrollArea* sc = new QScrollArea;
        sc->setWidget(wgt);
        wgt=sc;
    }else if(mode==1){
//        TableFormPlusData* tbForm = new TableFormPlusData(mapForm[str]);
//        tbForm->setFixedSize(87*x,79*y);
//        wgt = (QWidget*) tbForm;
    }*/


}

void ClassWidget::setFormClassed(QString nm){
    FormPlusData* formData = new FormPlusData;
    if (setting->getType()=="Вход"){
        formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/enter/Form"+nm+".cfpoq");
    }else if (setting->getType()=="Выход"){
        formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/exit/Form"+nm+".cfpoq");
    }else{
        formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/Form"+nm+".cfpoq");
    }
    mapForm[nm]=formData;
    int n = cls->classData[nm]->getSize();

    for (int i=0;i<n;i++){//пошли по всем полям
        QString str = "fldQ_"+QString::number(i+1);//берем название каждого последующего вопроса
        if (mapForm[nm]->contains(str)){//если есть такая ячейка
            CellPlusClass* cell = mapForm[nm]->getCell(str);
            cell->setValue(cls->classData[nm]->getBall(i));//то поставили значение в соответствующее поле
         }else{//если такого поля нет. Действуем следующим образом:
            QString val=cls->classData[nm]->getBall(i);//значение которое есть
            QStringList lst = val.split(";");
//======================================================
            int j=1;
            QString strC = str+"_"+QString::number(j);
            while (mapForm[nm]->contains(strC)){
                CellPlusClass* cell = mapForm[nm]->getCell(strC);
                if (cell->getTypeCell()=="Checkbox"){//в этом случае запоминаем номер
//                    if (lst.contains(QString::number(j))){//если есть номер этой ячейки в баллах
                    if (lst.contains(cell->getFormula())){//если есть ответ с таким текстом
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
    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(mapForm[nm]);
    ScrollWidget* sc = new ScrollWidget(ltf);

/*    QWidget* wgt = new QWidget;
    wgt->setLayout(ltf);
    QScrollArea* sc = new QScrollArea;
    sc->setWidget(wgt);
    wgt=sc;*/
    tab->addTab(sc,setting->getDiscipline(nm)->getNameDiscipline());
}

void ClassWidget::slotClose(){
    emit closed();
}

void ClassWidget::slotSaveAndClose(){
    tab->setCurrentIndex(0);//костыль
    slotSaveThis();
    slotClose();
}

void ClassWidget::slotAddStudent(){
    QString typePl = cls->getSetting()->getTypePl().split(";")[0].trimmed().toLower();
    DialogAddStudent* dlg = new DialogAddStudent(typePl, cls->getSetting());
    if (dlg->exec()==QDialog::Accepted){
        StudentClass* stud = new StudentClass(0,setting);
        stud->setCode(dlg->form->getCell("txtCode")->getValue());
        stud->setFio(dlg->form->getCell("txtFio")->getValue());
        cls->addStudent(stud);//добавили школьника в класс
        cls->signalResizeStudent();
        slotRefreshForms();
    }
}

void ClassWidget::slotRemoveStudent(){
    QTableWidgetItem* item = tsw->item(tsw->currentRow(), 0);
    if (item!=0){
        QString typePl = cls->getSetting()->getTypePl().split(";")[0].trimmed().toLower();
        DialogYouSure* dlg = new DialogYouSure(typePl);
        if (dlg->exec()==QDialog::Accepted){
            cls->removeStudent(item->text());
            tsw->refresh();
            cls->signalResizeStudent();
        }
    }
}

void ClassWidget::slotOpenStudent(StudentClass *stud){
    slotSaveThis();
    emit signalLoadStudent(stud);//работает для SQL
    emit signalOpenStudent(stud);
}

void ClassWidget::slotActivate(){
    emit signalActivate();
}

void ClassWidget::slotRefreshForms(){
    //tab->removeTab(0);
    slotSaveThis();
    tsw->refresh();
    int n = tab->count();//количество элементов
    int start = setting->getDisciplines("Classed").count()+1;
    for (int i=start;i<n;i++){
        tab->removeTab(start);
    }

    if (cls->hasMainForm){
        FormPlusData* formD  = mapForm["FormList"];
        mapForm.remove("FormList");
        delete formD;
        if (mMapWgt.contains("FormList")){
            QWidget* wgt = mMapWgt["FormList"];
            mMapWgt.remove("FormList");
            delete wgt;
        }
        setFormMainEnter();//добавили форму со списком класса если она есть
    }

    QList<QString> lst = cls->mapHasForm.uniqueKeys();
    for (int i=0;i<lst.count();i++){
        if (cls->mapHasForm[lst[i]]){
            FormPlusData* formD  = mapForm[lst[i]];
            mapForm.remove(lst[i]);
            delete formD;
            if (mMapWgt.contains(lst[i])){
                QWidget* wgt = mMapWgt[lst[i]];
                mMapWgt.remove(lst[i]);
                delete wgt;
            }
            setForm(lst[i]);//добавили форму по имени
        }
    }

}

void ClassWidget::slotAddMainForm(){
    if (!cls->hasMainForm){
        setFormMainEnter();
        cls->hasMainForm=true;
    }
}

void ClassWidget::slotAddForm(){

    slotSaveThis();//сначала сохранили
    QAction* actAddForm = (QAction*)sender();//отправитель
    QString str = mapActAdd.key(actAddForm);
            //actAddForm->text();//название кнопки//пока содержание, нужно будет заменить
    if (!cls->mapHasForm[str]){
        setForm(str);
        cls->mapHasForm[str]=true;
    }
}

void ClassWidget::slotDulicate(QString str){
    QMessageBox* msb = new QMessageBox;
    msb->setWindowTitle("Ошибка");
    msb->setText("Ученик с кодом \""+str+"\" уже есть!");
    msb->show();
}

void ClassWidget::slotSaveThis(){
    slotSave();
    emit signalSave();
}

void ClassWidget::slotSave(){
//записываем все изменения по классу
    QString code = form->getCell("txtClassCode")->getValue();
    if (code!="" && form->getCell("txtClassNumber")->getValue()!=""){
        int sz = setting->getOtherSetting("szgrpcode").toInt();
        if (sz>0 && code.length()!=sz){
            QMessageBox* msb = new QMessageBox(this);
            msb->setWindowTitle("Ошибка");
            QString typeGr = setting->getTypeGr().split(";")[1];
            msb->setText("Количество символов в коде "+typeGr+" должно равняться "+setting->getOtherSetting("szgrpcode")+"!");
            msb->show();
            return;
        }
        cls->setCode(code);
        cls->setNumber(form->getCell("txtClassNumber")->getValue());
    }else{
        QMessageBox* msb = new QMessageBox;
        msb->setWindowTitle("Ошибка!");
        msb->setText("Код и название не могут быть пустыми!");
        msb->show();
        return;
    }

//данные по классу
    QList<QString> lst = setting->getDisciplines("Classed").uniqueKeys();
    qDebug()<<"classed"<<lst;
    for (int i=0;i<lst.count();i++){
        //if (!mMapWgt.contains(lst[i])) continue;//если не показывали такую форму отпросто пропускаем при сохранении
        int n = cls->classData[lst[i]]->getSize();//количество вопросов в анкете
        for (int j=0;j<n;j++){
            QString str = "fldQ_"+QString::number(j+1);//берем название каждого последующего вопроса
            if (mapForm[lst[i]]->contains(str)){//если есть такая ячейка
                CellPlusClass* cell = mapForm[lst[i]]->getCell(str);
                cls->classData[lst[i]]->setBall(j, cell->getValue());
            }else{//если такого поля нет. Действуем следующим образом:
                QString val="";//значение которое будем записывать в студента
                int k=1;
                QString strC = str+"_"+QString::number(k);
                while (mapForm[lst[i]]->contains(strC)){
                    CellPlusClass* cell = mapForm[lst[i]]->getCell(strC);
                    if (cell->getTypeCell()=="Checkbox"){//в этом случае запоминаем номер/все равно запоминаем значение
                        if (cell->getValue()=="1"){
                            if (val.length()==0){
                                //val=QString::number(k);
                                val=cell->getFormula();
                            }else{
                                //val=val+";"+QString::number(k);
                                val=val+";"+cell->getFormula();
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
                cls->classData[lst[i]]->setBall(j, val);
                if (k==1){
                    qWarning()<<"небыло найдено такое поле. Форма"<<lst[i]<<"Поле"<<str;
                }
            }
        }

    }

//по каждому ребенку
    int n = cls->count;
    QVector<StudentClass*> vec = cls->getAllStudents();
    lst = setting->getDisciplines("Studented").uniqueKeys()<<setting->getDisciplines("MoreData").uniqueKeys();
    for (int i=0;i<n;i++){
        QString code = "_"+vec[i]->getCode();//код ученика
        if (cls->hasMainForm){
            vec[i]->setSex(mapForm["FormList"]->getCell("cmbSex"+code)->getValue());
            vec[i]->setBithday(mapForm["FormList"]->getCell("numDate"+code)->getValue());

            //идем по всем возможным анкетам и работам
            for (int j=0;j<lst.count();j++){
                vec[i]->setMaked(lst[j],mapForm["FormList"]->getCell("cmb"+lst[j]+code)->getValue());

            }
        }
        //по всем имеющимся формам
        QList<QString> lstF = cls->mapHasForm.uniqueKeys();
        for (int j=0;j<lstF.count();j++){
            if (cls->mapHasForm[lstF[j]] && vec[i]->getMaked(lstF[j])=="1" && mMapWgt.contains(lstF[j])){//если такая форма есть и она заполнена для ученика
                if (mapForm[lstF[j]]->contains("numVariant"+code)){
                    vec[i]->balls[lstF[j]]->setVariant(mapForm[lstF[j]]->getCell("numVariant"+code)->getValue());
                }
                int sz =  vec[i]->balls[lstF[j]]->getSize();
                for (int jf=0;jf<sz;jf++){
                    QString nameCell="numNumTask"+QString::number(jf+1)+code;
                    vec[i]->balls[lstF[j]]->setBall(jf,mapForm[lstF[j]]->getCell(nameCell)->getValue());
                }
            }
        }
    }
}


void ClassWidget::slotResults(){
    emit signalSave();
    if (!FileHelper::runTestJar()){
        DialogAttention* dlgAtt = new DialogAttention;
        if (dlgAtt->exec()!=QDialog::Accepted){
            delete dlgAtt;
            return;
        }
        delete dlgAtt;
    }

    //выбираем какие результаты нужны
    //по конкретной работе или по всем
    QVector<QString> vecSurv = KeyHelper::readMaybeLevels();
    //получили системные имена всех возможных анкет для которых есть уровни
    if (vecSurv.size()==0){
        ErrorMessage::showError("Нет данных об уровнях освоения!");
        return;
    }
    QMap<QString, DisciplineClass*> mapD = setting->getDisciplines("Shared");
    //список всех общих анкет (для которых вообще могут быть уровни)
    QStringList lst;
    QList<QString> keys = mapD.uniqueKeys();
    foreach (QString key, keys){
        if (!vecSurv.contains(key)) continue;
        DisciplineClass* dis = mapD[key];
        lst<<dis->getNameDiscipline();
    }
    lst<<"Все";

    //нужно получить все работы для которых есть ключи

    DialogSelectOnList* dlg = new DialogSelectOnList(lst);
    if (dlg->exec()==QDialog::Accepted){
        //чтото выбрали
        int pos = dlg->position;
        QList<QString> lstForms = cls->mapHasForm.uniqueKeys();//какие формы добавлены
        if (pos<vecSurv.size()){
            if (lstForms.contains(vecSurv[pos])){
                SurveyKeysClass* surv = KeyHelper::readKeyClass(vecSurv[pos]);
                KeyHelper::createCharacters(cls, surv);
                KeyHelper::createPdf(cls, QVector<SurveyKeysClass*>()<<surv);
                FileHelper::showResults();
            }else{
                ErrorMessage::showError("Форма анкеты не добавлена!");
            }
        }else{
            KeyHelper::createAllCharacters(cls, KeyHelper::readKeyClasses());
            int sz_not=0;
            for (int i=0;i<vecSurv.size();i++){
                if (lstForms.contains(vecSurv[i])){
                    SurveyKeysClass* surv = KeyHelper::readKeyClass(vecSurv[i]);
                    //KeyHelper::createCharacters(cls, surv);
                    KeyHelper::createPdf(cls, QVector<SurveyKeysClass*>()<<surv);
                }else{
                    sz_not++;
                }
            }
            if (sz_not){
                ErrorMessage::showError("Есть незаполненные анкеты ("+QString::number(sz_not)+")!");
            }
            FileHelper::showResults();
        }

    }
    delete dlg;
}

void ClassWidget::slotUpdateCompleted(){
    slotSave();
    emit signalSave();
    tab->setCurrentIndex(0);
    tsw->refresh();
}

void ClassWidget::slotShow(int n){

    if (n==mMapWgtPos["FormList"] && !mMapWgt.contains("FormList") && mapForm.contains("FormList")){
        LayoutToForm* ltf = new LayoutToForm;
        ltf->addFormData(mapForm["FormList"]);
        ScrollWidget* sc = new ScrollWidget(ltf);
        mMapWgt["FormList"]=sc;
        tab->insertTab(n,sc,"Основные данные");
        tab->setCurrentWidget(sc);
        if (tab->count()>n+1){
            tab->removeTab(n+1);
        }
    }else if(mMapWgtPos.values().contains(n)){
        //если это один из всех остальных
        QString str = mMapWgtPos.key(n);
        if (!mapForm.contains(str) || mMapWgt.contains(str)){
            //если нет такой формы или виджет уже добавлен
            return;
        }
        TableFormWidget* tbForm = new TableFormWidget(mapForm[str]);
        tbForm->setFixedSize(87*x,75*y);
        mMapWgt[str]=tbForm;

        tab->insertTab(n,tbForm,setting->getDiscipline(str)->getNameDiscipline());
        tab->setCurrentWidget(tbForm);
        if (tab->count()>n+1){
            tab->removeTab(n+1);
        }

        //    TableFormWidget* tbForm = new TableFormWidget(mapForm[str]);
        //    tbForm->setFixedSize(87*x,75*y);
        //    wgt = (QWidget*) tbForm;
    }
}

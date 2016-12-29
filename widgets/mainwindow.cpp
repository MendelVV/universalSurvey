#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mdiArea = new QMdiArea;
    SettingClass* setting = new SettingClass("open");
    setWindowTitle(setting->getTittle());

    nav = new Navigator;
    y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    QHBoxLayout* layout = new QHBoxLayout;
    nav->setFixedSize(6*x,88*y);
    mdiArea->setFixedSize(92*x,88*y);
    layout->addWidget(nav,0,Qt::AlignTop);
    layout->addWidget(mdiArea,0,Qt::AlignTop);
    QWidget* wgt = new QWidget;
    wgt->setLayout(layout);
    setCentralWidget(wgt);

    QMenu* menu = new QMenu("&Файл");
    setActMenu();
    menu->addAction(actCreate);
    menu->addSeparator();
    menu->addAction(actOpen);
//    menu->addAction(actSave);
    menu->addSeparator();
    menu->addAction(actOpenXML);
    menu->addSeparator();
    menu->addAction(actExit);
    menuBar()->addMenu(menu);
    menuBar()->addSeparator();
    menuBar()->addAction(actAuthor);

    QPalette palMain;
    QColor color(245, 245, 250);
    palMain.setColor(QPalette::Background,color);
    setPalette(palMain);
}


void MainWindow::setActMenu(){
    actCreate = new QAction("Новый",0);
    connect(actCreate,SIGNAL(triggered()),this, SLOT(slotCreateSchool()));

    actOpen = new QAction("Открыть",0);
    connect(actOpen,SIGNAL(triggered()),this,SLOT(slotOpen()));

    actOpenXML = new QAction("Открыть XML",0);
    connect(actOpenXML,SIGNAL(triggered()),this,SLOT(slotOpenXML()));

    actSave = new QAction("Сохранить",0);

    actExit = new QAction ("Выйти",0);
    connect(actExit, SIGNAL(triggered()),this,SLOT(close()));

    actAuthor = new QAction("О программе",0);
    connect(actAuthor,SIGNAL(triggered(bool)),this,SLOT(slotAuthor()));
}

void MainWindow::slotSave(){
    emit signalSave();
}

void MainWindow::slotCreateSchool(){
    SchoolClass* sch = new SchoolClass();
    SchoolWidget* swgt = new SchoolWidget(sch);
    QMdiSubWindow* subw = mdiArea->addSubWindow(swgt);
    subw->setWindowFlags(Qt::FramelessWindowHint);
    swgt->showMaximized();
//соединяем со слотом, чтобы при закрытии закрывалось все
    connect(swgt,SIGNAL(closed()),this,SLOT(slotCloseSubWindow()));
    connect(swgt,SIGNAL(signalOpenClass(ClassClass*)),this,SLOT(slotOpenClass(ClassClass*)));
    connect(swgt,SIGNAL(signalActivate()),this,SLOT(slotActivateSubWindow()));
    connect(this,SIGNAL(signalSave()),swgt,SLOT(slotSave()));
    nav->addItem(sch);
}

void MainWindow::slotOpenClass(ClassClass *cls){
    if (!nav->contains(cls)){
        ClassWidget* cwgt = new ClassWidget(cls);
        QMdiSubWindow* subw = mdiArea->addSubWindow(cwgt);
        subw->setWindowFlags(Qt::FramelessWindowHint);
        cwgt->showMaximized();
    //соединяем со слотами
        connect(cwgt,SIGNAL(closed()),this,SLOT(slotCloseSubWindow()));
    //для открытия школьника
        connect(cwgt,SIGNAL(signalOpenStudent(StudentClass*)),this,SLOT(slotOpenStudent(StudentClass*)));
        connect(cwgt,SIGNAL(signalActivate()),this,SLOT(slotActivateSubWindow()));
        connect(cwgt,SIGNAL(signalSave()),this,SLOT(slotSave()));
        nav->addItem(cls);
    }else{
        cls->signalActiveView();
    }
}

void MainWindow::slotOpen(){
    QFileDialog* openD = new QFileDialog(this);
    QString str = openD->getOpenFileName(0,"Выберите файл","","");
    if (str.length()>0 && str.contains(".scpd")){
        SchoolClass* sch = new SchoolClass;
        sch->readSchoolClass(str);
        SchoolWidget* swgt = new SchoolWidget(sch);
        swgt->fileName=str;
        mdiArea->addSubWindow(swgt);
        swgt->showMaximized();
        activateWindow();
    //соединяем со слотом, чтобы при закрытии закрывалось все
        connect(swgt,SIGNAL(closed()),this,SLOT(slotCloseSubWindow()));
        connect(swgt,SIGNAL(signalOpenClass(ClassClass*)),this,SLOT(slotOpenClass(ClassClass*)));
        connect(swgt,SIGNAL(signalActivate()),this,SLOT(slotActivateSubWindow()));
        connect(this,SIGNAL(signalSave()),swgt,SLOT(slotSave()));
        nav->addItem(sch);
    }
}

void MainWindow::slotOpenXML(){
    QFileDialog* openD = new QFileDialog(this);
    QString str = openD->getOpenFileName(0,"Выберите файл","","");
    if (str.length()>0 && str.contains(".xml")){
        SchoolClass* sch = new SchoolClass;
        sch->readSchoolXML(str);
        SchoolWidget* swgt = new SchoolWidget(sch);
        swgt->fileName=str.replace(".xml",".scpd");
        mdiArea->addSubWindow(swgt);
        swgt->showMaximized();
        activateWindow();
    //соединяем со слотом, чтобы при закрытии закрывалось все
        connect(swgt,SIGNAL(closed()),this,SLOT(slotCloseSubWindow()));
        connect(swgt,SIGNAL(signalOpenClass(ClassClass*)),this,SLOT(slotOpenClass(ClassClass*)));
        connect(swgt,SIGNAL(signalActivate()),this,SLOT(slotActivateSubWindow()));
        connect(this,SIGNAL(signalSave()),swgt,SLOT(slotSave()));
        nav->addItem(sch);
    }
}

void MainWindow::slotOpenStudent(StudentClass *stud){
    if (!nav->contains(stud)){
        StudentWidget* stwgt = new StudentWidget(stud);
        QMdiSubWindow* subw = mdiArea->addSubWindow(stwgt);
        subw->setWindowFlags(Qt::FramelessWindowHint);
        stwgt->showMaximized();
        connect(stwgt,SIGNAL(closed()),this,SLOT(slotCloseSubWindow()));
        connect(stwgt,SIGNAL(signalActivate()),this,SLOT(slotActivateSubWindow()));
        connect(stwgt,SIGNAL(signalSave()),this,SLOT(slotSave()));
        nav->addItem(stud);
    }else{
        stud->signalActiveView();
    }
}

void MainWindow::slotCloseSubWindow(){
    mdiArea->closeActiveSubWindow();


    mdiArea->activatePreviousSubWindow();
    QWidget* pview = (QWidget*) sender()->parent();
    delete pview;
}

void MainWindow::slotActivateSubWindow(){
    QMdiSubWindow* subWindow = (QMdiSubWindow*) sender()->parent();
    mdiArea->setActiveSubWindow(subWindow);
}

void MainWindow::slotAuthor(){
    QFont font("Times",11,QFont::Normal);
    QString link ="https://www.github.com/MendelVV/universalSurvey";
    QTextBrowser* brouser = new QTextBrowser;

    brouser->setFont(font);
    brouser->setWindowTitle("О программе");
    QString str = "<html><head><title></title></head><body><div><center><font color='black' size='4'>Сбор анкетых данных 2.0.1.2</font></center>";
    str = str+"<p align='justify'><font color='black'>Приложение разработано Менделем В.В. на основе библиотеки QT (версия 5.5.1) и предназначено для offline "
                  "сбора данных о различного рода анкетированиях.";
    str = str+"<br><br>Приложения относится к СПО. С исходным кодом можно ознакомиться на <a href='"+link+"'>GitHub</a>.";
    str=str+"<br><br>Контактная информация для сотрудничества по созданию анкет и обработке анкетных данных: ";
    str=str+"<br>E-mail:</font> <font color='blue'>mendel.vasilij@yandex.ru</font></p>";
    str=str+"<center>2016 г. Версия 2.0.1.2</center></div></body></html>";
    brouser->setHtml(str);
    brouser->setOpenExternalLinks(true);

//    lbl->setAlignment(Qt::AlignJustify);
    brouser->setFixedWidth(40*x);
    brouser->resize(40*x,30*y);
    brouser->show();

}

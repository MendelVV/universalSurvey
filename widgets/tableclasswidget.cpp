#include "tableclasswidget.h"

TableClassWidget::TableClassWidget(SchoolClass* school, QWidget *parent) :
    QTableWidget(parent)
{
    sch = school;
    connect(sch,SIGNAL(signalAddClass(ClassClass*,int)),this,SLOT(slotAddRow(ClassClass*,int)));
    setAcceptDrops(true);//разрешили драг-анд-дроп
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    insertColumn(0);
    insertColumn(1);
    insertColumn(2);
    setColumnWidth(0,13*x);//задали ширину нулевой колонки
    setColumnWidth(1,13*x);//задали ширину первой колонки
    setColumnWidth(2,13*x);//задали ширину второй колонки
    QStringList lst;
    QString typeGr = sch->getSetting()->getTypeGr().split(";")[0].trimmed().toLower();
    typeGr = sch->getSetting()->toFirstUpper(typeGr);
    QString typePl = sch->getSetting()->getTypePl().split(";")[1].trimmed().toLower();
    typePl = sch->getSetting()->toFirstUpper(typePl);
    lst<<"Код"<<typeGr<<typePl;
    setHorizontalHeaderLabels(lst);
    int n = school->getAllClasses().count();
    QVector<ClassClass*> vec = school->getAllClasses();
    for (int i=0;i<n;i++){
        addRow(vec[i]);
    }
    reprint();
    setFixedSize(x*45,y*68);
}

void TableClassWidget::addRow(ClassClass *cls){
    int n = rowCount();
    insertRow(n);
    QTableWidgetItem* item;
    item = new QTableWidgetItem(cls->getCode());
    setItem(n,0,item);
    item = new QTableWidgetItem(cls->getNumber());
    setItem(n,1,item);
    item = new QTableWidgetItem(QString::number(cls->getAllStudents().count()));
    setItem(n,2,item);
}

void TableClassWidget::reprint(){
    int rows = this->rowCount();
    for (int i=0;i<rows;i++){
        int columns = columnCount();
        for (int j=0;j<columns;j++){
            if (i%2==1){
                item(i,j)->setBackgroundColor(QColor(235, 240, 255));
            }else{
                item(i,j)->setBackgroundColor(QColor(255, 255, 255));
            }
        }
    }
//    if (n%2==1){
//        item->setBackgroundColor(QColor(235, 240, 255));
//    }
}

void TableClassWidget::refresh(){
    int countRow = rowCount();
    for (int i=0;i<countRow;i++){
        removeRow(0);
    }
    int n = sch->count;
    QVector<ClassClass*> vec = sch->getAllClasses();
    for (int i=0;i<n;i++){
        addRow(vec[i]);
    }
    reprint();
}

void TableClassWidget::mousePressEvent(QMouseEvent *event){
     if (event->buttons()==1){
        QTableWidgetItem* tbItem = itemAt(event->pos());
        if (tbItem!=0){
            selectRow(tbItem->row());
        }
     }
}

void TableClassWidget::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->buttons()==1){
        QTableWidgetItem* tbItem = itemAt(event->pos());
        if (tbItem!=0){
            QString str =item(currentRow(),0)->text();
            ClassClass* cls = sch->getClass(str);
            if (cls!=0){
                emit signalOpenClass(cls);
//                ClassWidget* clw = new ClassWidget(cls);
//                clw->showMaximized();
            }
//                emit signalOpenObject(pro->getPObject(str));//отправили сигнал что нужно открыть объект

        }
    }
}

void TableClassWidget::slotAddRow(ClassClass *cls, int n){
    insertRow(n);
    QTableWidgetItem* item;
    item = new QTableWidgetItem(cls->getCode());
    setItem(n,0,item);
    item = new QTableWidgetItem(cls->getNumber());
    setItem(n,1,item);
    item = new QTableWidgetItem(QString::number(cls->getAllStudents().count()));
    setItem(n,2,item);
    reprint();
}

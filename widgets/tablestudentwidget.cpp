#include "tablestudentwidget.h"

TableStudentWidget::TableStudentWidget(ClassClass* classC, QWidget *parent) :
    QTableWidget(parent)
{
   cls = classC;
   connect(cls,SIGNAL(signalAddStudent(StudentClass*,int)),this,SLOT(slotAddRow(StudentClass*,int)));
   setAcceptDrops(true);//разрешили драг-анд-дроп
   double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
   double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
   insertColumn(0);
   insertColumn(1);
   insertColumn(2);
   insertColumn(3);
   setColumnWidth(0,10*x);//задали ширину нулевой колонки
   setColumnWidth(1,20*x);//задали ширину первой колонки
   setColumnWidth(2,5*x);//задали ширину второй колонки
   setColumnWidth(3,5*x);
   QStringList lst;
   QString typeGr = cls->getSetting()->getTypeGr().split(";")[0].trimmed().toLower();
   typeGr = cls->getSetting()->toFirstUpper(typeGr);
   lst<<"Код"<<"ФИО"<<typeGr<<"Полнота";
   setHorizontalHeaderLabels(lst);
   int n = cls->count;
   QVector<StudentClass*> vec = cls->getAllStudents();
   for (int i=0;i<n;i++){
       addRow(vec[i]);
   }
   reprint();
   setFixedSize(x*45,y*68);
   contMenu = new QMenu;
   QAction* actEdit =new QAction("Изменить",0);
   connect(actEdit,SIGNAL(triggered(bool)),this,SLOT(slotEdit()));
   contMenu->addAction(actEdit);
}

void TableStudentWidget::addRow(StudentClass *stud){
    int n = rowCount();
    insertRow(n);
    QTableWidgetItem* item;
    item = new QTableWidgetItem(stud->getCode());
    setItem(n,0,item);
    item = new QTableWidgetItem(stud->getFio());
    setItem(n,1,item);
    item = new QTableWidgetItem(cls->getNumber());
    setItem(n,2,item);

/*    qDebug()<<"pre get maked";
    QMap<QString, QString> mapMaked = stud->getMaked();
    QList<QString> keys = mapMaked.uniqueKeys();
    double total = 0;
    double completed = 0;
    qDebug()<<keys;

    foreach(QString key, keys){
        qDebug()<<"key ="<<key<<mapMaked[key];
        if (mapMaked[key]=="0") continue;

        BallsClass* ballC = stud->balls[key];
        int n = ballC->getSize();
        total+=n;
        qDebug()<<"total ="<<total;
        for (int i=0;i<n;i++){
            if (ballC->getBall(i)!="") completed++;
        }
    }
    double res = 0;
    if (total>0){
        qDebug()<<"last total ="<<total;
        res = completed/total;
    }*/

    item = new QTableWidgetItem(doubleToPercent(stud->getCompleteness()));
    setItem(n,3,item);
}

QString TableStudentWidget::doubleToPercent(double value){
    double res = value*100;
    QString r = QString::number(res);
    if (r.contains(".")){
        int pos = r.indexOf(".");
        r=r.mid(0,pos);
    }
    r+="%";
    return r;
}

void TableStudentWidget::reprint(){
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
}

void TableStudentWidget::refresh(){
    int countRow = rowCount();
    for (int i=0;i<countRow;i++){
        removeRow(0);
    }

    int n = cls->count;
    QVector<StudentClass*> vec = cls->getAllStudents();
    for (int i=0;i<n;i++){
        addRow(vec[i]);
    }
    reprint();
}

void TableStudentWidget::mousePressEvent(QMouseEvent *event){
     if (event->buttons()==1){
        QTableWidgetItem* tbItem = itemAt(event->pos());
        if (tbItem!=0){
            selectRow(tbItem->row());
        }
     }else if (event->buttons()==2){
         QTableWidgetItem* tbItem = itemAt(event->pos());
         if (tbItem!=0){
             selectRow(tbItem->row());
         }
     }
}

void TableStudentWidget::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->buttons()==1){
        QTableWidgetItem* tbItem = itemAt(event->pos());
        if (tbItem!=0){
            QString str =item(currentRow(),0)->text();
            StudentClass* stud = cls->getStudent(str);
            if (stud!=0){
                emit signalOpenStudent(stud);
            }
//                emit signalOpenObject(pro->getPObject(str));//отправили сигнал что нужно открыть объект

        }
    }
}

void TableStudentWidget::contextMenuEvent(QContextMenuEvent *ce){
    contMenu->exec(ce->globalPos());
}

void TableStudentWidget::slotAddRow(StudentClass *stud, int n){
    insertRow(n);
    QTableWidgetItem* item;
    item = new QTableWidgetItem(stud->getCode());
    setItem(n,0,item);
    item = new QTableWidgetItem(stud->getFio());
    setItem(n,1,item);
    item = new QTableWidgetItem(cls->getNumber());
    setItem(n,2,item);
    item = new QTableWidgetItem(doubleToPercent(stud->getCompleteness()));
    setItem(n,3,item);
    reprint();
}

void TableStudentWidget::slotEdit(){
    QTableWidgetItem* tbItem = item(currentRow(),0);
    if (tbItem!=0){
        QString str =item(currentRow(),0)->text();
        StudentClass* stud = cls->getStudent(str);
        if (stud!=0){
            QString typePl = cls->getSetting()->getTypePl().split(";")[0].trimmed().toLower();
            DialogAddStudent* dlg = new DialogAddStudent(typePl, cls->getSetting());
            dlg->form->getCell("txtCode")->setValue(stud->getCode());
            dlg->form->getCell("txtCode")->changeValueWidget();
            dlg->form->getCell("txtFio")->setValue(stud->getFio());
            dlg->form->getCell("txtFio")->changeValueWidget();
            if (dlg->exec()==QDialog::Accepted){
                stud->setFio(dlg->form->getCell("txtFio")->getValue());
                if (stud->getCode()!=dlg->form->getCell("txtCode")->getValue()){
                    if (cls->contains(dlg->form->getCell("txtCode")->getValue())){
                        return;
                    }
                    cls->removeStudent(stud->getCode());
//                    refresh();
                    stud->setCode(dlg->form->getCell("txtCode")->getValue());
                    cls->addStudent(stud);
                }
                refresh();
            }
            delete dlg;
        }
    }
}

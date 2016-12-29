#include "tabledisciplinewidget.h"

TableDisciplineWidget::TableDisciplineWidget(QWidget *parent) :
    QTableWidget(parent)
{

}

TableDisciplineWidget::TableDisciplineWidget(SettingClass* st, QWidget* parent) :
    QTableWidget(parent)
{
    setting=st;
    connect(setting,SIGNAL(signalAddDiscipline()),this,SLOT(slotRefresh()));
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    double x =((double)GetSystemMetrics(SM_CXSCREEN))/100;
    insertColumn(0);
    insertColumn(1);
    insertColumn(2);
    insertColumn(3);
    setColumnWidth(0,10*x);//задали ширину нулевой колонки
    setColumnWidth(1,15*x);//задали ширину первой колонки
    setColumnWidth(2,9*x);//задали ширину второй колонки
    setColumnWidth(3,6*x);//задали ширину третьей колонки
    QStringList lst;
    lst<<"Системное имя"<<"Название"<<"Тип"<<"Заданий";
    setHorizontalHeaderLabels(lst);
    setFixedSize(x*42,y*60);
    slotRefresh();

    contMenu = new QMenu();
    QAction* actRemove = new QAction("Удалить",0);
    connect(actRemove,SIGNAL(triggered(bool)),this,SLOT(slotRemove()));
    contMenu->addAction(actRemove);
}

void TableDisciplineWidget::addDiscipline(DisciplineClass *dsp){
//добавляем строку
    int n = rowCount();
    insertRow(n);
    QTableWidgetItem* item;
    item = new QTableWidgetItem(dsp->getNameSystem());
    setItem(n,0,item);
    item = new QTableWidgetItem(dsp->getNameDiscipline());
    setItem(n,1,item);
    item = new QTableWidgetItem(dsp->getType());
    setItem(n,2,item);
    item = new QTableWidgetItem(QString::number(dsp->getCount()));
    setItem(n,3,item);
}

void TableDisciplineWidget::removeDiscipline(QString nm){
    setting->removeDiscipline(setting->getDiscipline(nm));
    slotRefresh();
}

void TableDisciplineWidget::slotRefresh(){
    int n = this->rowCount();
    for (int i=0;i<n;i++){
        removeRow(0);
    }

    QList<DisciplineClass*> lstD = setting->getDisciplines().values();
    n = lstD.count();
    for (int i=0;i<n;i++){
        addDiscipline(lstD[i]);
    }
    reprint();
}

void TableDisciplineWidget::reprint(){
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

void TableDisciplineWidget::mousePressEvent(QMouseEvent *event){
     if (event->buttons()==1){
        QTableWidgetItem* tbItem = itemAt(event->pos());
        if (tbItem!=0){
            selectRow(tbItem->row());
        }
     }else if (event->buttons()==2){
         QTableWidgetItem* tbItem = itemAt(event->pos());
         if (tbItem!=0){
             //лупим контекстное меню для удаления строки
             selectRow(tbItem->row());
             contMenu->exec(event->globalPos());
         }
     }
}

void TableDisciplineWidget::mouseDoubleClickEvent(QMouseEvent *event){
    if (event->buttons()==1){
        QTableWidgetItem* tbItem = item(currentRow(),0);
        if (tbItem!=0){
            DisciplineClass* dsp = setting->getDiscipline(tbItem->text());
            DisciplineWidget* dspw = new DisciplineWidget(dsp);
            if (dspw->exec()==QDialog::Accepted){
                if (dspw->form->getValue("fldNameSystem")!="" && dspw->form->getValue("fldNameDiscipline")!=""){
                    dsp->setNameSystem(dspw->form->getValue("fldNameSystem"));
                    dsp->setNameDiscipline(dspw->form->getValue("fldNameDiscipline"));
                    dsp->setType(dspw->form->getValue("fldType"));
                    dsp->setCount(dspw->form->getValue("fldCount").toInt());
                    slotRefresh();
                }else{
                    QMessageBox* msb = new QMessageBox();
                    msb->setWindowTitle("Ошибка");
                    msb->setText("Анкета/Дисциплина не может изменена. Не хватает названия.");
                    msb->show();
                }
            }
        }
    }
}

void TableDisciplineWidget::slotRemove(){
    QTableWidgetItem* tbItem = item(currentRow(),0);
    if (tbItem!=0){
        removeDiscipline(tbItem->text());
    }
}

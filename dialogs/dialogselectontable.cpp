#include "dialogselectontable.h"

DialogSelectOnTable::DialogSelectOnTable(QVector<QVector<QString> > vecData , QStringList lst, QVector<double> vecSz, QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Произведите выбор");

    y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    x =((double)GetSystemMetrics(SM_CXSCREEN))/100;

    form = new FormPlusData;
    CellPlusClass* cell;
    cell = new CellPlusClass("lblSearch","Label","Поиск",0,0,10);
    form->addCell(cell);
    cell = new CellPlusClass("fldSearch","Text","",0,1,28);
    form->addCell(cell);
    cell = new CellPlusClass("butSearch","Button","Найти",0,2,10);
    connect(cell,SIGNAL(signalPress()),this,SLOT(slotSearch()));
    form->addCell(cell);

    cell = new CellPlusClass("lblCode","Label","Код",2,0,10);
    form->addCell(cell);
    cell = new CellPlusClass("lblNumber","Label","Название",2,1,28);
    form->addCell(cell);
    cell = new CellPlusClass("lblAdd","Label","Если нет в списке",2,2,10);
    form->addCell(cell);

    cell = new CellPlusClass("fldCode","Text","",3,0,10);
    form->addCell(cell);
    cell = new CellPlusClass("fldNumber","Text","",3,1,28);
    form->addCell(cell);
    cell = new CellPlusClass("butAdd","Button","Добавить новую",3,2,10);
    connect(cell,SIGNAL(signalPress()),this,SLOT(slotAdd()));
    form->addCell(cell);


    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);

    mVecData = vecData;
    if (vecData.size()==0)return;
    mTable = new QTableWidget(mVecData.size(),mVecData[0].size());
    mTable->setHorizontalHeaderLabels(lst);
    refresh();


    ltf->addWidget(mTable,1,0,1,4);
    setLayout(ltf);
    connect(mTable,SIGNAL(cellDoubleClicked(int,int)),this,SLOT(slotSelect(int, int)));
    setMinimumSize(50*x,50*y);
    for (int i=0;i<mVecData[0].size();i++){
        double dx = 50*x*vecSz[i]/100;
        mTable->setColumnWidth(i, dx);
    }
}

DialogSelectOnTable::~DialogSelectOnTable(){
    delete form;
}

QVector<QString> DialogSelectOnTable::getValue(){
    return mValue;
}

void DialogSelectOnTable::refresh(){
    int n = mVecData.size();

    for (int i=0;i<n;i++){
        int m = mVecData[i].size();
        for (int j=0;j<m;j++){
            QTableWidgetItem* tItem = new QTableWidgetItem(mVecData[i][j]);
            Qt::ItemFlags flags = tItem->flags();
            flags |=Qt::ItemIsSelectable;
            flags &=~Qt::ItemIsEditable;
            tItem->setFlags(flags);
            mTable->setItem(i,j,tItem);
        }
    }
}

int DialogSelectOnTable::searchRow(QString code){
    int pos = mVecData.size()/2;
    int step = pos;
    while (step>1){
        if (mVecData.size()<=pos) break;
        if (mVecData[pos][0].length()<code.length()){
            break;
        }
        QString val = mVecData[pos][0].left(code.length());
        step = (step+step%2)/2;
        if (val<code){
            pos=pos+step;
        }else if (val>code){
            pos=pos-step;
        }else{
            //совпали
            break;
        }

    }
    //известно примерное положение
    return pos;
}

void DialogSelectOnTable::slotSelect(int r, int){
    if (r<mVecData.size()){
        mValue = mVecData[r];
        accept();
    }
}

void DialogSelectOnTable::slotSearch(){
    //ищем в таблице
    int pos = searchRow(form->getValue("fldSearch"));
    mTable->selectRow(pos);
}

void DialogSelectOnTable::slotAdd(){
    int pos = searchRow(form->getValue("fldCode"));
    if (mVecData[pos][0]==form->getValue("fldCode")){
        //такое уже есть
        ErrorMessage::showError("Школа с таким кодом уже есть! Кликните дважды, чтобы выбрать.");
        mTable->selectRow(pos);
    }else{
        mValue.clear();
        mValue<<form->getValue("fldCode")<<form->getValue("fldNumber");
        accept();
    }
}

#include "navigator.h"

Navigator::Navigator(QWidget *parent) :
    QListWidget(parent)
{
}

bool Navigator::contains(SchoolClass* sch){
    return listSch.contains(sch);
}

bool Navigator::contains(ClassClass* cls){
    return listCls.contains(cls);
}

bool Navigator::contains(StudentClass* stud){
    return listStud.contains(stud);
}

void Navigator::addItem(SchoolClass* sch){
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    QLinearGradient gradient(0,0,0,5*y);//(0, 0, 0, opt->rect.height());
    gradient.setSpread(QGradient::PadSpread);
    gradient.setColorAt(0, QColor(220,220,255));
    gradient.setColorAt(0.5, QColor(180,180,255));
    gradient.setColorAt(1, QColor(230,230,255));

    listSch.append(sch);
    MiniView* mv = new MiniView(sch);
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setBackground(QBrush(gradient));
   // item->setBackgroundColor(QColor(255,240,240));
    mapItemSch[sch]=item;
    item->setSizeHint(mv->size());
    setItemWidget(item,mv);
//соединяем сигнал закрытия школы с удалением из навигатора
    connect(sch,SIGNAL(closedView(SchoolClass*)),this,SLOT(slotRemove(SchoolClass*)));

    countItems++;
}

void Navigator::addItem(ClassClass* cls){
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    QLinearGradient gradient(0,0,0,5*y);//(0, 0, 0, opt->rect.height());
    gradient.setSpread(QGradient::PadSpread);
    gradient.setColorAt(0, QColor(215,235,255));
    gradient.setColorAt(0.5, QColor(190,210,255));
    gradient.setColorAt(1, QColor(170,190,210));

    listCls.append(cls);
    MiniView* mv = new MiniView(cls);
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setBackground(QBrush(gradient));
   // item->setBackgroundColor(QColor(255,240,240));
    mapItemCls[cls]=item;
    item->setSizeHint(mv->size());
    setItemWidget(item,mv);
//соединяем сигнал закрытия школы с удалением из навигатора
    connect(cls,SIGNAL(closedView(ClassClass*)),this,SLOT(slotRemove(ClassClass*)));
    countItems++;
}

void Navigator::addItem(StudentClass* stud){
    double y =((double)GetSystemMetrics(SM_CYSCREEN))/100;
    QLinearGradient gradient(0,0,0,5*y);//(0, 0, 0, opt->rect.height());
    gradient.setSpread(QGradient::PadSpread);
    gradient.setColorAt(0, QColor(230,255,230));
    gradient.setColorAt(0.5, QColor(200,255,200));
    gradient.setColorAt(1, QColor(170,210,170));

    listStud.append(stud);
    MiniView* mv = new MiniView(stud);
    QListWidgetItem* item = new QListWidgetItem(this);
    item->setBackground(QBrush(gradient));
   // item->setBackgroundColor(QColor(255,240,240));
    mapItemStud[stud]=item;
    item->setSizeHint(mv->size());
    setItemWidget(item,mv);
//соединяем сигнал закрытия ученика с удалением из навигатора
    connect(stud,SIGNAL(closedView(StudentClass*)),this,SLOT(slotRemove(StudentClass*)));

    countItems++;
}

void Navigator::slotRemove(SchoolClass* sch){
    removeItemWidget(mapItemSch[sch]);
    delete mapItemSch[sch];
    mapItemSch.remove(sch);
    listSch.removeOne(sch);
    countItems--;
}

void Navigator::slotRemove(ClassClass* cls){
    removeItemWidget(mapItemCls[cls]);
    delete mapItemCls[cls];
    mapItemCls.remove(cls);
    listCls.removeOne(cls);
    countItems--;
}

void Navigator::slotRemove(StudentClass* stud){
    removeItemWidget(mapItemStud[stud]);
    delete mapItemStud[stud];
    mapItemStud.remove(stud);
    listStud.removeOne(stud);
    countItems--;
}

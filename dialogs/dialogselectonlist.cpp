#include "dialogselectonlist.h"

DialogSelectOnList::DialogSelectOnList(QStringList lst, QWidget *parent) :
    QDialog(parent)
{
    setModal(true);
    setWindowTitle("Выберите из списка");
    QListWidget* listW = new QListWidget;
    listW->addItems(lst);
    value="";
    connect(listW,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(slotAccept()));
    QHBoxLayout* lt = new QHBoxLayout;
    lt->addWidget(listW);
    setLayout(lt);
}

void DialogSelectOnList::slotAccept(){
    QListWidget* listW = (QListWidget*) sender();
    position = listW->currentRow();
    value = listW->currentItem()->text();
    accept();
}

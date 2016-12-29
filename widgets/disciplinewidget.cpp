#include "disciplinewidget.h"

DisciplineWidget::DisciplineWidget(QWidget *parent) : QDialog(parent)
{
    setModal(true);
    form = new FormPlusData;
    CellPlusClass* cell;
    cell = new CellPlusClass("lblNameSystem", "Label", "Системное имя", 0, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldNameSystem", "Text", "", 0, 1, 15, 4);
    form->addCell(cell);

    cell = new CellPlusClass("lblNameDiscipline", "Label", "Название анкеты/дисциплины", 1, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldNameDiscipline", "Text", "", 1, 1, 15, 4);
    form->addCell(cell);

    cell = new CellPlusClass("lblType", "Label", "Тип анкеты/дисциплины", 2, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldType", "Combobox", "", 2, 1, 15, 4);
    cell->setList(QStringList()<<"Индивидуальная"<<"На группу"<<"Общая");
    form->addCell(cell);

    cell = new CellPlusClass("lblCount", "Label", "Количество заданий", 3, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldCount", "NumberInt", "", 3, 1, 15, 4);
    form->addCell(cell);

    cell = new CellPlusClass("butAdd", "Button", "Добавить", 4, 0, 10, 4);
    connect(cell,SIGNAL(signalPress()),this,SLOT(accept()));
    form->addCell(cell);

    cell = new CellPlusClass("butCancel", "Button", "Отменить", 4, 1, 10, 4);
    connect(cell,SIGNAL(signalPress()),this,SLOT(reject()));
    form->addCell(cell);
    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);
    setLayout(ltf);
}

DisciplineWidget::DisciplineWidget(DisciplineClass* dsp, QWidget *parent) : QDialog(parent)
{
    setModal(true);
    form = new FormPlusData;
    CellPlusClass* cell;
    cell = new CellPlusClass("lblNameSystem", "Label", "Системное имя", 0, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldNameSystem", "Text", dsp->getNameSystem(), 0, 1, 15, 4);
    form->addCell(cell);

    cell = new CellPlusClass("lblNameDiscipline", "Label", "Название анкеты/дисциплины", 1, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldNameDiscipline", "Text", dsp->getNameDiscipline(), 1, 1, 15, 4);
    form->addCell(cell);

    cell = new CellPlusClass("lblType", "Label", "Тип анкеты/дисциплины", 2, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldType", "Combobox", dsp->getType(), 2, 1, 15, 4);
    cell->setList(QStringList()<<"Индивидуальная"<<"На группу"<<"Общая");
    form->addCell(cell);

    cell = new CellPlusClass("lblCount", "Label", "Количество заданий", 3, 0, 12, 4);
    form->addCell(cell);

    cell = new CellPlusClass("fldCount", "NumberInt", QString::number(dsp->getCount()), 3, 1, 15, 4);
    form->addCell(cell);

    cell = new CellPlusClass("butAdd", "Button", "Сохранить", 4, 0, 10, 4);
    connect(cell,SIGNAL(signalPress()),this,SLOT(accept()));
    form->addCell(cell);

    cell = new CellPlusClass("butCancel", "Button", "Отменить", 4, 1, 10, 4);
    connect(cell,SIGNAL(signalPress()),this,SLOT(reject()));
    form->addCell(cell);
    LayoutToForm* ltf = new LayoutToForm;
    ltf->addFormData(form);
    setLayout(ltf);
}

DisciplineWidget::~DisciplineWidget(){
    delete form;
}

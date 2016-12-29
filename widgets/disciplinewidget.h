#ifndef DISCIPLINEWIDGET_H
#define DISCIPLINEWIDGET_H

#include "layouttoform.h"
#include "disciplineclass.h"

class DisciplineWidget : public QDialog
{
    Q_OBJECT
public:
    explicit DisciplineWidget(QWidget *parent = 0);
    explicit DisciplineWidget(DisciplineClass* dsp, QWidget *parent = 0);
    ~DisciplineWidget();
    FormPlusData* form;
signals:

public slots:
};

#endif // DISCIPLINEWIDGET_H

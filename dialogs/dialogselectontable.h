#ifndef DIALOGSELECTONTABLE_H
#define DIALOGSELECTONTABLE_H

#include <layouttoform.h>
#include "dialogs/errormessage.h"

class DialogSelectOnTable : public QDialog
{
    Q_OBJECT
public:
    DialogSelectOnTable(QVector<QVector<QString> > vecData , QStringList lst, QVector<double> vecSz, QWidget *parent = 0);
    ~DialogSelectOnTable();
    QVector<QString> getValue();
protected:
    FormPlusData* form;
    QTableWidget* mTable;
    QVector<QVector<QString> > mVecData;
    QVector<QString> mValue;
    double x;
    double y;

    void refresh();
    int searchRow(QString text);
public slots:
    void slotSelect(int r, int c);
    void slotSearch();
    void slotAdd();
};

#endif // DIALOGSELECTONTABLE_H

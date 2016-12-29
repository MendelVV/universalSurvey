#ifndef DIALOGYOUSURE_H
#define DIALOGYOUSURE_H

#include <QDialog>
#include "layouttoform.h"

class DialogYouSure : public QDialog
{
    Q_OBJECT
public:
    explicit DialogYouSure(QString str, QWidget *parent = 0);

signals:

public slots:

};

#endif // DIALOGYOUSURE_H

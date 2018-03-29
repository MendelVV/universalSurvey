#ifndef DIALOGCLOSE_H
#define DIALOGCLOSE_H

#include <QtWidgets>

class DialogClose : public QDialog
{
    Q_OBJECT
public:
    explicit DialogClose(QWidget *parent = 0);
    bool mSave;
signals:

public slots:
    void slotSave();
};

#endif // DIALOGCLOSE_H

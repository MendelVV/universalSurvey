#ifndef DIALOGSELECTONLIST_H
#define DIALOGSELECTONLIST_H

#include <QtWidgets>

class DialogSelectOnList : public QDialog
{
    Q_OBJECT
public:
    explicit DialogSelectOnList(QStringList lst, QWidget *parent = 0);
    QString value;
    int position;
signals:

public slots:
    void slotAccept();
};

#endif // DIALOGSELECTONLIST_H

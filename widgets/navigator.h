#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include <QtWidgets>
#include <QLinearGradient>
#include "schoolclass.h"
#include "miniview.h"

class Navigator : public QListWidget//класс являющийся меню слева (отрывное). используется для навигации по объектам
{
    Q_OBJECT
public:
    explicit Navigator(QWidget *parent = 0);
    virtual void addItem(SchoolClass* sch);
    virtual void addItem(ClassClass* cls);
    virtual void addItem(StudentClass* stud);

    bool contains(SchoolClass* sch);
    bool contains(ClassClass* cls);
    bool contains(StudentClass* stud);

    int size();

protected:
    int countItems;
    QMap<SchoolClass*, QListWidgetItem*> mapItemSch;
    QMap<ClassClass*, QListWidgetItem*> mapItemCls;
    QMap<StudentClass*, QListWidgetItem*> mapItemStud;
    QList<SchoolClass*> listSch;//список открытых объектов
    QList<ClassClass*> listCls;//список открытых объектов
    QList<StudentClass*> listStud;//список открытых проектов
//ключи и основное окно (не картами, а просто так есть, так как они в единственном экземпляре)
signals:

public slots:
    void slotSave();
    void slotSaveAll();
    void slotRemove(SchoolClass*);
    void slotRemove(ClassClass*);
    void slotRemove(StudentClass*);
};

#endif // NAVIGATOR_H

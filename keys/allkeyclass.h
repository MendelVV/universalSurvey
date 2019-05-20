#ifndef ALLKEYCLASS_H
#define ALLKEYCLASS_H

#include "keys_data/surveykeysclass.h"
#include "classclass.h"
#include "keys/resultsdata.h"

class AllKeyClass
{
public:
    //класс для проверки значений по одной анкете
    AllKeyClass(SurveyKeysClass* survKey);
    double checkQuestion(int num, QString value, int var);
    double checkSolve(BallsClass *ballC, int start, int end, int type);
    QString getLevel(StudentClass* stud);
    double getSumBalls(StudentClass* stud, int level=-1);//сколько набрано баллов за эту работу
    QVector<double> getLevelsComplete(StudentClass* stud);//сколько баллов за какую часть набрано
    ResultsData* getResultData(ClassClass* cls);//нужные сведения о выполнениии
private:
    SurveyKeysClass* mSurvKey;
    QMap<int, QuestionKey*> mMapKeys;
    QMap<int, double> mMapMax;//словарь максимальных баллов
    double mMaxBall;
};

#endif // ALLKEYCLASS_H

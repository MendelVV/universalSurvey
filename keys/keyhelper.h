#ifndef KEYHELPER_H
#define KEYHELPER_H

#include "json/jsonhelper.h"
#include "keys_data/surveykeysclass.h"
#include <QCoreApplication>
#include "classclass.h"
#include "keys/allkeyclass.h"
#include "filehelper.h"
#include <QPdfWriter>
#include <QPainter>
#include "formplusdata.h"
class KeyHelper
{
public:
    KeyHelper();
    static QVector<QString> readMaybeLevels();//получаем названия всех возможных анкет для проверки
    static QVector<SurveyKeysClass*> readKeyClasses();//считываем все ключи и уровни
    static SurveyKeysClass* readKeyClass(QString survName);//считываем только для одной анкеты

    static QJsonObject createDataFromSurvey(ClassClass* cls, SurveyKeysClass* survKey);
//    static QString getLevel(BallsClass* ball, QVector<SubLevel*> levels);

    static void createCharacters(ClassClass* cls, SurveyKeysClass* surv);
    static void createAllCharacters(ClassClass* cls, QVector<SurveyKeysClass*> vecSurvs);

    static void createPdf(ClassClass* cls, QVector<SurveyKeysClass*> vecSurvs);
    static void createOnePdfOld(ClassClass* cls, SurveyKeysClass* surv);
    static void createOnePdf(ClassClass* cls, SurveyKeysClass* surv);
    static QVector<QVector<QString> > getDataForTable(ClassClass* cls, SurveyKeysClass* surv,AllKeyClass* allKeys, ResultsData* results);
protected:
    static void createNextPage(QPdfWriter* writer, QPainter* painter, int page, ResultsData* results, ClassClass* cls, QVector<QVector<QString> > vecData);

};

#endif // KEYHELPER_H

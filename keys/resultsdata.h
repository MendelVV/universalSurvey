#ifndef RESULTSDATA_H
#define RESULTSDATA_H

#include <QMap>
#include <QVector>

class ResultsData
{
public:
    ResultsData();

    QMap<QString, double> mMapLevelsP;//процент по уровням
    QMap<QString, QVector<double> > mVecBalls;//выполнение заданий (если -1 то не выполнял)

    QVector<double> mVecClsBalls;//процент выполнения задания
};

#endif // RESULTSDATA_H

#include "allkeyclass.h"

AllKeyClass::AllKeyClass(SurveyKeysClass* survKey)
{
    mSurvKey = survKey;
    QVector<QuestionKey*> keys = mSurvKey->getKeys();
    foreach(QuestionKey* key, keys){
        mMapKeys[key->pos-1]=key;
    }
    //создали словарь с вопросами по порядку
    //хотим узнать максимум баллов за работу

}

double AllKeyClass::checkQuestion(int num, QString value, int var){
    //значений может быть как одно так и список
    //ищем правильный ключ

    value = value.toLower();
    if (value=="" || value=="n"){//если нет ответа
        return 0;
    }

    QuestionKey* qkey = mMapKeys[num];//правильный ключ
    if (qkey->isBall){
        return -1;//сказали что это балл и ничего не нужно проверять
    }
    if (var==0){//всего один вариант
        if(qkey->sum){//если нужно суммировать все ответы
            QStringList lst = value.split(";");//получили все ответы отдельно
            int n = lst.count();
            double sum = 0;
            for (int i=0;i<n;i++){
                sum = sum+qkey->check(lst[i]);
            }
            return sum;
        }else{
            return qkey->check(value);
        }
    }else{//есть неколько вариантов
        if(qkey->sum){//если нужно суммировать все ответы
            QStringList lst = value.split(";");//получили все ответы отдельно
            int n = lst.count();
            double sum = 0;
            for (int i=0;i<n;i++){
                sum = sum+qkey->check(lst[i],var);
            }
            return sum;
        }
        else{
            return qkey->check(value,var);
        }
    }
}

double AllKeyClass::checkSolve(BallsClass *ballC, int start, int end, int type){
    double res = 0;
    for (int i=start-1;i<end;i++){
        //начиная с start-1 и до end-1
        double ball = checkQuestion(i,ballC->getBall(i),ballC->getVariant().toInt());
        if (ball==-1){
            ball=ballC->getBall(i).toDouble();
        }
        switch (type) {
        case 0:{
            if (ball!=0){
                res++;
            }
            break;
        }
        case 1:{
            res = res+ball;
            break;
        }
        default:
            break;
        }
    }
    return res;
}

QString AllKeyClass::getLevel(StudentClass* stud){
     //теперь точно верный код анкеты, проверяем

    if (stud->getMaked(mSurvKey->getSurvey())!="1"){
        //если не выполнял то N
        return "N";
    }
    BallsClass* balls = stud->balls[mSurvKey->getSurvey()];
    QVector<SubLevel*> levels = mSurvKey->getLevels();

    QStringList lstRes;
    foreach (SubLevel* subLevel, levels) {
        int n = subLevel->count();
        bool b = true;
        for (int j=0;j<n;j++){
            QMap<QString, QString> map = subLevel->getLine(j);//взяли строчку условий
            //проверяем выполняется ли
            int start = map["start"].toInt();
            int end = map["end"].toInt();
            int type = -1;
            if (map["type"]=="Решить заданий"){
                type=0;
            }else if (map["type"]=="Набрать баллов"){
                type=1;
            }
            QStringList lstCount = map["countv"].split("-");
            if (lstCount.count()!=2){
                qDebug()<<"not 2";
                b=false;
                break;
            }
            int count_min = lstCount[0].toInt();
            int count_max = lstCount[1].toInt();
            double res = (int) checkSolve(balls,start,end,type);

            if (res<count_min || res>count_max){
                b = false;
                break;
            }
        }
        if (b){//если подошло
            if (!lstRes.contains(subLevel->getText())){//и такого еще нет
                lstRes<<subLevel->getText();
            }
        }
    }

    int val = -1;//неудовлетворительно
    QString result = "";
    foreach (QString str, lstRes){
        int lv = 0;
        if (str=="Низкий"){
            lv=0;
        }else if(str=="Пониженный"){
            lv=1;
        }else if(str=="Базовый"){
            lv=2;
        }else if(str=="Повышенный"){
            lv=3;
        }else if(str=="Высокий"){
            lv=4;
        }
        if (val<lv){
            val = lv;
            result = str;
        }
    }

    return result;
}

double AllKeyClass::getSumBalls(StudentClass *stud, int level){
    double res = 0;
    if (stud->getMaked(mSurvKey->getSurvey())!="1"){
        //если не выполнял то N
        return 0;
    }

    BallsClass* balls = stud->balls[mSurvKey->getSurvey()];
    int sz = balls->getSize();
    for (int i=0;i<sz;i++){
        if (level==0 && mMapKeys[i]->level!=0) continue;
        if (level==1 && mMapKeys[i]->level!=1) continue;
        double ball = checkQuestion(i,balls->getBall(i),balls->getVariant().toInt());
        if (ball>=0){
            res+=ball;
        }else{
            res+=balls->getBall(i).toDouble();
        }
    }

    return res;
}

ResultsData* AllKeyClass::getResultData(ClassClass *cls){
    ResultsData* results = new ResultsData();

    QVector<StudentClass*> vecStd = cls->getAllStudents();
    double total = 0;
    QMap<QString, double> mapLevels;
    QMap<int, double> mapClsRes;//сколько сделало какое задание
    QMap<QString, QVector<double> > mapBalls;
    foreach(StudentClass* stud, vecStd){
        QString level = getLevel(stud);
        //получили уровень
        if (level=="N"){
            //не выполнял работу
            continue;
        }
        total++;
        if (mapLevels.contains(level)){
            mapLevels[level]=mapLevels[level]+1;
        }else{
            mapLevels[level]=1;
        }
        QVector<double> vecRes;
        BallsClass* balls = stud->balls[mSurvKey->getSurvey()];
        int sz = balls->getSize();
        for (int i=0;i<sz;i++){
            if (balls->getBall(i).toLower()=="n"){
                vecRes<<-1;//сказали что не выполнялась работа
                continue;
            }
            double ball = checkQuestion(i,balls->getBall(i),balls->getVariant().toInt());
            if (ball<0){
                ball = balls->getBall(i).toDouble();
            }
            vecRes<<ball;
            if (ball>0){
                if (mapClsRes.contains(i)){
                    mapClsRes[i]=mapClsRes[i]+1;
                }else{
                    mapClsRes[i]=1;
                }
            }
        }
        mapBalls[stud->getCode()]=vecRes;
    }
    results->mVecBalls = mapBalls;
    QVector<double> vecClsBall;
    int sz = mSurvKey->getKeys().size();
    for (int i=0;i<sz;i++){
        if (mapClsRes.contains(i)){
            double p = mapClsRes[i]/total;
            vecClsBall<<p;
        }else{
            vecClsBall<<0;
        }
    }
    results->mVecClsBalls=vecClsBall;
    QList<QString> keys = mapLevels.uniqueKeys();
    foreach (QString key, keys){
        double p = mapLevels[key]/total;
        mapLevels[key]=p;
    }

    results->mMapLevelsP=mapLevels;
    return results;
}

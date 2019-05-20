#include "keyhelper.h"

KeyHelper::KeyHelper()
{

}

QVector<QString> KeyHelper::readMaybeLevels(){
    QVector<QString> vecRes;

    QString str=QCoreApplication::applicationDirPath()+"/keys.json";

    QFile file(str);
    if (!file.exists()){
        return vecRes;
    }

    QJsonObject jsonObject = JsonHelper::readJson(str);
    QJsonArray arrObj = jsonObject["keys"].toArray();
    int n = arrObj.size();
    for (int i=0;i<n;i++){
        QJsonObject jsonKey = arrObj.at(i).toObject();
        vecRes<<jsonKey["survey"].toString();
    }
    return vecRes;
}

QVector<SurveyKeysClass*> KeyHelper::readKeyClasses(){
    QVector<SurveyKeysClass*> vecRes;
    QString str=QCoreApplication::applicationDirPath()+"/keys.json";

    QFile file(str);
    if (!file.exists()){
        return vecRes;
    }

    QJsonObject jsonObject = JsonHelper::readJson(str);
    QJsonArray arrObj = jsonObject["keys"].toArray();
    int n = arrObj.size();
    for (int i=0;i<n;i++){
        SurveyKeysClass* survKey = new SurveyKeysClass();
        survKey->fromJson(arrObj.at(i).toObject());
        vecRes<<survKey;
    }
    return vecRes;
}


SurveyKeysClass* KeyHelper::readKeyClass(QString survName){
    QString str=QCoreApplication::applicationDirPath()+"/keys.json";
    QJsonObject jsonObject = JsonHelper::readJson(str);
    QJsonArray arrObj = jsonObject["keys"].toArray();
    SurveyKeysClass* survKey = new SurveyKeysClass();
    int n = arrObj.size();
    for (int i=0;i<n;i++){
        QJsonObject jsonKey = arrObj.at(i).toObject();
        if (jsonKey["survey"].toString()==survName){
           survKey->fromJson(jsonKey);
        }
    }
    return survKey;
}

QJsonObject KeyHelper::createDataFromSurvey(ClassClass* cls, SurveyKeysClass* survKey){
    QJsonObject jsonObject;
    QVector<StudentClass*> vecStd = cls->getAllStudents();
    QMap<QString, double> mapPercent;//в качестве ключа код ученика
    QMap<QString, double> mapPercentBasic;//в качестве ключа код ученика
    QMap<QString, double> mapPercentHight;//в качестве ключа код ученика
    QMap<QString, int> mapLevels;
    AllKeyClass* allKeysClass = new AllKeyClass(survKey);//класс с помощью которого будем все считать
    QJsonArray arrPercent;
    QJsonArray arrPercentBasic;
    QJsonArray arrPercentHight;

    int total = 0;
    foreach (StudentClass* stud, vecStd) {
        //идем по всем студентам

        //определяем уровень

        QString level = allKeysClass->getLevel(stud);
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
        //определяем процент выполнени работы
        QJsonObject jsonPercent;
        double sum = allKeysClass->getSumBalls(stud);
//        mapPercent[stud->getCode()]=100.0*sum/survKey->getMaxBall();//процент выполнения работы
        jsonPercent["code"]=stud->getCode();
        jsonPercent["value"]=100.0*sum/survKey->getMaxBall();
        arrPercent.append(jsonPercent);

        //определем сколько баллов из какой части набрано
        QJsonObject jsonPercentBasic;
        double sumBasic = allKeysClass->getSumBalls(stud,0);
//        mapPercentBasic[stud->getCode()]=100.0*sumBasic/survKey->getMaxBasic();//процент выполнения базовой части
        jsonPercentBasic["code"]=stud->getCode();
        jsonPercentBasic["value"]=100.0*sumBasic/survKey->getMaxBasic();
        arrPercentBasic.append(jsonPercentBasic);

        QJsonObject jsonPercentHight;
        double sumHight = allKeysClass->getSumBalls(stud,1);
        //mapPercentHight[stud->getCode()]=100.0*sumHight/survKey->getMaxHight();//процент выполнения сложной части
        jsonPercentHight["code"]=stud->getCode();
        jsonPercentHight["value"]=100.0*sumHight/survKey->getMaxHight();
        arrPercentHight.append(jsonPercentHight);


        //для pdf нужна табличка выполнения заданий
    }
    jsonObject["arrPercent"]=arrPercent;
    jsonObject["arrPercentBasic"]=arrPercentBasic;
    jsonObject["arrPercentHight"]=arrPercentHight;
    //проценты выполнения
    jsonObject["total"]=total;//всего человек которые написали
    jsonObject["survey"]=survKey->getSurvey();//системное имя анкеты
    //заполняем уровни
    QList<QString> list = mapLevels.uniqueKeys();
    QJsonArray arrLevels;
    foreach (QString str, list){
        QJsonObject jsonLevel;
        jsonLevel["level"]=str;
        jsonLevel["size"]=mapLevels[str];
        arrLevels.append(jsonLevel);
    }
    jsonObject["levels"]=arrLevels;
    //объект по анкете готов
    return jsonObject;
}

void KeyHelper::createCharacters(ClassClass* cls, SurveyKeysClass *surv){
    //создаем json файл и говорим java начать дествовать
    QString str=QCoreApplication::applicationDirPath()+"/charts";
    FileHelper::makeDir("charts");

    QJsonObject jsonObject;
    QJsonArray arrSurvs;

    arrSurvs.append(createDataFromSurvey(cls,surv));
    jsonObject["surveys"]=arrSurvs;
    QFile file(str+"/charts_data.json");//в папку приложения
    if (!file.open(QIODevice::WriteOnly)){
        qWarning()<<"no open file!";
        return;
    }

    QJsonDocument saveDoc(jsonObject);

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("CP1251"));
    stream<<saveDoc.toJson();
//                file.write(saveDoc.toJson());
    file.close();
}

void KeyHelper::createAllCharacters(ClassClass* cls, QVector<SurveyKeysClass*> vecSurvs){
    //так создаем для всех анкет
    QString str=QCoreApplication::applicationDirPath()+"/charts";
    FileHelper::makeDir(str);

    QJsonObject jsonObject;
    QJsonArray arrSurvs;
    foreach (SurveyKeysClass* surv, vecSurvs){
        arrSurvs.append(createDataFromSurvey(cls,surv));
    }
    jsonObject["surveys"]=arrSurvs;
    QFile file(str+"/charts_data.json");//в папку приложения
    if (!file.open(QIODevice::WriteOnly)){
        qWarning()<<"no open file!";
        return;
    }

    QJsonDocument saveDoc(jsonObject);

    QTextStream stream(&file);
    stream.setCodec(QTextCodec::codecForName("CP1251"));
    stream<<saveDoc.toJson();
//                file.write(saveDoc.toJson());
    file.close();
}

void KeyHelper::createPdf(ClassClass* cls, QVector<SurveyKeysClass*> vecSurvs){
    //запускаем jar
    FileHelper::runJar();
    //диаграмы созданы
    //тперь для каждой анкеты создаем pdf файл
    FileHelper::makeDir("results");
    foreach (SurveyKeysClass* surv, vecSurvs) {
        createOnePdf(cls,surv);
    }
}

void KeyHelper::createOnePdfOld(ClassClass* cls, SurveyKeysClass* survs){
    SettingClass* settingClass = cls->getSetting();
    AllKeyClass* allKeys = new AllKeyClass(survs);
    ResultsData* results = allKeys->getResultData(cls);//получили все нужные данные
    //считаем уровни


    //создаем файл pdf
    QString pth=QCoreApplication::applicationDirPath();
    //+"/resutls/";
    QString nm = settingClass->getDiscipline(survs->getSurvey())->getNameDiscipline();
    QString fileName = pth+"/results/результаты_"+nm+".pdf";

    QFont font12("Times New Roman",18);
    QFont font10("Times New Roman",14);
    QFont font8("Times New Roman",8);

    QPdfWriter writer(fileName);//создали документ pdf в выбранной папке
    writer.setPageOrientation(QPageLayout::Portrait);
    writer.setPageSize(QPagedPaintDevice::A4);//задали размеры страницы
    QPainter painter(&writer);//создали объект которым будем рисовать
    QPen pen(Qt::black);//цвет ручки
    pen.setWidth(10);//размер текста
    painter.setPen(pen);


    double wx = ((double)writer.width())/100;//1% по x
    double wy = ((double)writer.height())/100;//1% по y
    double x;
    double y = wy*2;

    QTextOption to(Qt::AlignCenter);
    painter.setFont(font12);

    painter.drawText(QRect(0,y,100*wx,2*wy),"Результаты по дисциплине \""+nm+"\"",to);
    y+=5*wy;
    painter.setFont(font10);
    painter.drawText(QRect(0,y,100*wx,2*wy),"Разделение по уровням освоения",to);
    y+=3*wy;

    QPixmap imagePie(pth+"/charts/"+survs->getSurvey()+"_pie.png");

    double sz_w = wx*50;
    double sz_h = sz_w*7/11;

    painter.drawPixmap(wx*50, y, sz_w, sz_h,imagePie);

    //слева рисуем таблицу с уровнями
    font8.setBold(true);
    painter.setFont(font8);


    y+=6*wy;
    x = 8*wx;
    int ty = y-wy/2;
    painter.drawLine(x-wx,ty,x-wx,ty+12*wy);
    painter.drawLine(x+15*wx,ty,x+15*wx,ty+12*wy);
    painter.drawLine(x+32*wx,ty,x+32*wx,ty+12*wy);
    for (int i=0;i<7;i++){
        painter.drawLine(x-wx,ty,x+32*wx,ty);
        ty+=2*wy;
    }

    painter.drawText(QRect(x,y,15*wx,2*wy),"Уровень освоения");
    painter.drawText(QRect(x+16*wx,y,15*wx,2*wy),"Процент участников");
    font8.setBold(false);
    painter.setFont(font8);
    y+=2*wy;
    QStringList listLevels;
    listLevels<<"Низкий"<<"Пониженный"<<"Базовый"<<"Повышенный"<<"Высокий";

    foreach (QString lv, listLevels) {
        painter.drawText(QRect(x,y,15*wx,2*wy),lv);
        QString val = QString::number(results->mMapLevelsP[lv]*100);
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        val+="%";
        painter.drawText(QRect(x+16*wx,y,15*wx,2*wy),val);
        y+=2*wy;
    }

    //Дальше две диаграмы выполнения заданий
    y=10*wy+sz_h;

    painter.setFont(font10);
    painter.drawText(QRect(0,y,100*wx,2*wy),"Выполнение заданий",to);
    y+=4*wy;

    sz_w = 64*wx;
    sz_h = sz_w*7/11;

    QPixmap imageDots(pth+"/charts/"+survs->getSurvey()+"_dots.png");
    QPixmap imageHis(pth+"/charts/"+survs->getSurvey()+"_his.png");

    painter.drawPixmap(18*wx,y,sz_w,sz_h,imageDots);

    y+=sz_h+2*wy;

    painter.drawPixmap(18*wx,y,sz_w,sz_h,imageHis);

    //пошли на следующую страницу

    writer.setPageOrientation(QPageLayout::Landscape);
    writer.newPage();

    wx = ((double)writer.width())/100;//1% по x
    wy = ((double)writer.height())/100;//1% по y

    y = wy*2;

    painter.setFont(font12);
    painter.drawText(QRect(0,y,100*wx,2*wy),"Выполнение заданий",to);
    y+=8*wy;
    //дальше идет таблица со всеми результатами и даже больше
    //нужна форма мо предмету чтобы узнать названия заданий
    int sz = results->mVecClsBalls.size();//всего заданий
    FormPlusData* formData = new FormPlusData;
    formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/Form"+survs->getSurvey()+".cfpoq");
    //получили форму
    sz_w = 3*wx;
    sz_h = 3*wy;
//    lblNumTask3
    //ученик, список заданий

    QVector<StudentClass*> vecStd = cls->getAllStudents();
    QMap<int, double> mapTaskCompleted;//сколько каких заданий выполнено
    int total=0;
    int n = vecStd.size();
    double tx = 4*wx;

    painter.drawLine(tx,y,tx,y+(n+2)*sz_h);//левая граница
    tx+=11*wx;
    painter.drawLine(tx,y,tx,y+(n+2)*sz_h);//левая граница

    font8.setBold(true);
    painter.setFont(font8);

    x = 5*wx;
    painter.drawText(QRect(x,y,10*wx,3*wy),"Ученик",to);
    x+=10*wx;
    for (int i=0;i<sz;i++){
        QString val = formData->getValue("lblNumTask"+QString::number(i+1));
        painter.drawText(QRect(x,y,sz_w,3*wy),val,to);
        x+=sz_w;
        tx+=sz_w;
        painter.drawLine(tx,y,tx,y+(n+2)*sz_h);
    }
    painter.drawText(QRect(x,y,8*wx,3*wy),"Базовый %",to);
    x+=8*wx;
    painter.drawLine(x,y,x,y+(n+2)*sz_h);
    painter.drawText(QRect(x,y,8*wx,3*wy),"Повышенный %",to);
    x+=8*wx;
    painter.drawLine(x,y,x,y+(n+2)*sz_h);
    painter.drawText(QRect(x,y,8*wx,3*wy),"Общий %",to);
    x+=8*wx;
    painter.drawLine(x,y,x,y+(n+2)*sz_h);
    double tw = x;
    //дальше по всем ученикам
    font8.setBold(false);
    painter.setFont(font8);

    painter.drawLine(4*wx,y,tw,y);//верх

    painter.drawLine(4*wx,y+sz_h,tw,y+sz_h);//верх
    QTextOption to1(Qt::AlignVCenter);
    double p_b=0;//базовый
    double p_p=0;//повышенный
    double p_t=0;//общий
    foreach (StudentClass* stud, vecStd) {
        y+=sz_h;
        painter.drawLine(4*wx,y+sz_h,tw,y+sz_h);//верх
        x = 5*wx;
        painter.drawText(QRect(x,y,10*wx,3*wy),stud->getCode(),to1);
        x+=10*wx;
        if (stud->getMaked(survs->getSurvey())!="1"){
            for (int i=0;i<sz;i++) {
                painter.drawText(QRect(x,y,sz_w,3*wy),"N",to);
                x+=sz_w;
            }
            continue;
        }
        total++;
        int k=0;
        QVector<double> vecRes = results->mVecBalls[stud->getCode()];
        foreach (double ball, vecRes) {
            QString val;
            if (ball==-1){
                val="N";
            }else{
                if (ball>0){
                    mapTaskCompleted[k]+=1.0;
                }
                val = QString::number(ball);
            }
            k++;
            painter.drawText(QRect(x,y,sz_w,3*wy),val,to);
            x+=sz_w;
        }
        QString val;
        val = QString::number(100*allKeys->getSumBalls(stud,0)/survs->getMaxBasic());
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        painter.drawText(QRect(x,y,8*wx,3*wy),val,to);
        x+=8*wx;
        val = QString::number(100*allKeys->getSumBalls(stud,1)/survs->getMaxHight());
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        painter.drawText(QRect(x,y,8*wx,3*wy),val,to);
        x+=8*wx;
        val = QString::number(100*allKeys->getSumBalls(stud)/survs->getMaxBall());
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        painter.drawText(QRect(x,y,8*wx,3*wy),val,to);
    }
    y=y+sz_h;
    painter.drawLine(4*wx,y+sz_h,tw,y+sz_h);//низ
    //нужен низ таблицы
    x = 5*wx;
    painter.drawText(QRect(x,y,10*wx,3*wy),"Итого",to1);
    x+=10*wx;
    for (int i=0;i<sz;i++){
        double res = mapTaskCompleted[i]/total;
        QString val = QString::number(res*100);
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        val+="%";
        painter.drawText(QRect(x,y,sz_w,3*wy),val,to);
        x+=sz_w;
    }
    painter.drawText(QRect(x,y,8*wx,3*wy),"-",to);
    x+=8*wx;
    painter.drawText(QRect(x,y,8*wx,3*wy),"-",to);
    x+=8*wx;
    painter.drawText(QRect(x,y,8*wx,3*wy),"-",to);


    painter.end();
}

void KeyHelper::createOnePdf(ClassClass* cls, SurveyKeysClass* survs){
    SettingClass* settingClass = cls->getSetting();
    AllKeyClass* allKeys = new AllKeyClass(survs);
    ResultsData* results = allKeys->getResultData(cls);//получили все нужные данные
    //считаем уровни


    //создаем файл pdf
    QString pth=QCoreApplication::applicationDirPath();
    //+"/resutls/";
    QString nm = settingClass->getDiscipline(survs->getSurvey())->getNameDiscipline();
    QString fileName = pth+"/results/результаты_"+nm+".pdf";

    QFont font12("Times New Roman",18);
    QFont font10("Times New Roman",14);
    QFont font8("Times New Roman",8);

    QPdfWriter writer(fileName);//создали документ pdf в выбранной папке
    writer.setPageOrientation(QPageLayout::Portrait);
    writer.setPageSize(QPagedPaintDevice::A4);//задали размеры страницы
    QPainter painter(&writer);//создали объект которым будем рисовать
    QPen pen(Qt::black);//цвет ручки
    pen.setWidth(10);//размер текста
    painter.setPen(pen);


    double wx = ((double)writer.width())/100;//1% по x
    double wy = ((double)writer.height())/100;//1% по y
    double x;
    double y = wy*2;

    QTextOption to(Qt::AlignCenter);
    painter.setFont(font12);

    painter.drawText(QRect(0,y,100*wx,2*wy),"Результаты по дисциплине \""+nm+"\"",to);
    y+=5*wy;
    painter.setFont(font10);
    painter.drawText(QRect(0,y,100*wx,2*wy),"Разделение по уровням освоения",to);
    y+=3*wy;

    QFile file(pth+"/charts/"+survs->getSurvey()+"_pie.png");


    double sz_w = wx*50;
    double sz_h = sz_w*7/11;

    if (file.exists()){
        QPixmap imagePie(pth+"/charts/"+survs->getSurvey()+"_pie.png");
        painter.drawPixmap(wx*50, y, sz_w, sz_h,imagePie);
    }


    //слева рисуем таблицу с уровнями
    font8.setBold(true);
    painter.setFont(font8);


    y+=6*wy;
    x = 8*wx;
    int ty = y-wy/2;
    painter.drawLine(x-wx,ty,x-wx,ty+12*wy);
    painter.drawLine(x+15*wx,ty,x+15*wx,ty+12*wy);
    painter.drawLine(x+32*wx,ty,x+32*wx,ty+12*wy);
    for (int i=0;i<7;i++){
        painter.drawLine(x-wx,ty,x+32*wx,ty);
        ty+=2*wy;
    }

    painter.drawText(QRect(x,y,15*wx,2*wy),"Уровень освоения");
    painter.drawText(QRect(x+16*wx,y,15*wx,2*wy),"Процент участников");
    font8.setBold(false);
    painter.setFont(font8);
    y+=2*wy;
    QStringList listLevels;
    listLevels<<"Низкий"<<"Пониженный"<<"Базовый"<<"Повышенный"<<"Высокий";

    foreach (QString lv, listLevels) {
        painter.drawText(QRect(x,y,15*wx,2*wy),lv);
        QString val = QString::number(results->mMapLevelsP[lv]*100);
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        val+="%";
        painter.drawText(QRect(x+16*wx,y,15*wx,2*wy),val);
        y+=2*wy;
    }

    //Дальше две диаграмы выполнения заданий
    y=10*wy+sz_h;

    painter.setFont(font10);
    painter.drawText(QRect(0,y,100*wx,2*wy),"Выполнение заданий",to);
    y+=4*wy;

    if (cls->getAllStudents().size()>25){
        sz_w = 74*wx;
        sz_h = sz_w*7/13;
    }else{
        sz_w = 64*wx;
        sz_h = sz_w*7/11;
    }
    double dx = (100*wx-sz_w)/2;
    file.setFileName(pth+"/charts/"+survs->getSurvey()+"_dots.png");
    if (file.exists()){
        QPixmap imageDots(pth+"/charts/"+survs->getSurvey()+"_dots.png");
        painter.drawPixmap(dx,y,sz_w,sz_h,imageDots);
    }
    y+=sz_h+2*wy;

    file.setFileName(pth+"/charts/"+survs->getSurvey()+"_his.png");
    if (file.exists()){
        QPixmap imageHis(pth+"/charts/"+survs->getSurvey()+"_his.png");
        painter.drawPixmap(dx,y,sz_w,sz_h,imageHis);
    }

    //пошли на следующую страницу

    QVector<QVector<QString> > vecData = getDataForTable(cls,survs,allKeys,results);//данные

    writer.setPageOrientation(QPageLayout::Landscape);

    int max = 28;

    int pages = (cls->getAllStudents().size())/max;//количество страниц с учетом линии "итого"
    for (int i=0;i<=pages;i++){
        createNextPage(&writer,&painter,i,results,cls,vecData);
    }

/*    writer.newPage();

    wx = ((double)writer.width())/100;//1% по x
    wy = ((double)writer.height())/100;//1% по y

    y = wy*2;

    painter.setFont(font12);
    painter.drawText(QRect(0,y,100*wx,3*wy),"Выполнение заданий",to);
    y+=8*wy;
    //дальше идет таблица со всеми результатами и даже больше
    //нужна форма мо предмету чтобы узнать названия заданий
    int sz = results->mVecClsBalls.size();//всего заданий
    //получили форму
    sz_w = 3*wx;
    sz_h = 3*wy;
//    lblNumTask3
    //ученик, список заданий

    QVector<double> vecDx;
    QVector<StudentClass*> vecStd = cls->getAllStudents();
    int n = vecStd.size();

    font8.setBold(true);
    painter.setFont(font8);

    vecDx<<10*wx;
    QVector<QString> vecHeader = vecData[0];//заголовок
    for (int i=0;i<sz;i++){
        vecDx<<sz_w;
    }

    vecDx<<8*wx<<8*wx<<8*wx;
    int col=0;
    x = 5*wx;
    painter.drawLine(4*wx,y,4*wx,y+(n+2)*sz_h);//левая граница
    foreach (QString val, vecHeader) {
        painter.drawText(QRect(x,y,vecDx[col],3*wy),val,to);
        x+=vecDx[col];
        col++;
        painter.drawLine(x,y,x,y+(n+2)*sz_h);
    }

    double tw = x;//конец таблицы по x

    //дальше по всем ученикам
    font8.setBold(false);
    painter.setFont(font8);

    QTextOption to1(Qt::AlignVCenter);
    painter.drawLine(4*wx,y,tw,y);//верх
    y+=sz_h;
    int sz_lines = vecData.size();
    for (int i=1;i<sz_lines;i++){
        x = 5*wx;
        col=0;
        QVector<QString> line = vecData[i];
        foreach (QString val, line) {
            if (col==0){
                painter.drawText(QRect(x,y,vecDx[col],sz_h),val,to1);
            }else{
                painter.drawText(QRect(x,y,vecDx[col],sz_h),val,to);
            }

            x+=vecDx[col];
            col++;
        }
        painter.drawLine(4*wx,y,tw,y);//низ
        y+=sz_h;
    }

    painter.drawLine(4*wx,y,tw,y);//низ*/
    painter.end();
}

void KeyHelper::createNextPage(QPdfWriter* writer, QPainter* painter, int page, ResultsData* results, ClassClass* cls, QVector<QVector<QString> > vecData){

    int max = 28;

    QTextOption to(Qt::AlignCenter);

    QFont font12("Times New Roman",18);
    QFont font8("Times New Roman",8);

    writer->newPage();

    double wx = ((double)writer->width())/100;//1% по x
    double wy = ((double)writer->height())/100;//1% по y

    float y = wy*2;

    painter->setFont(font12);
    painter->drawText(QRect(0,y,100*wx,3*wy),"Выполнение заданий",to);
    y+=8*wy;
    //дальше идет таблица со всеми результатами и даже больше
    //нужна форма мо предмету чтобы узнать названия заданий
    int sz = results->mVecClsBalls.size();//всего заданий
    //получили форму
    double sz_w = 3*wx;
    double sz_h = 3*wy;
//    lblNumTask3
    //ученик, список заданий

    QVector<double> vecDx;
    QVector<StudentClass*> vecStd = cls->getAllStudents();
    int n =(vecStd.size()-max*page);//не более 30 учеников на странице
    if (n>max) n = max;
    int start = max*page;

    bool isLast = (vecStd.size()-(max*(page+1))<=0);
    if (isLast) n++;

    font8.setBold(true);
    painter->setFont(font8);

    vecDx<<10*wx;
    QVector<QString> vecHeader = vecData[0];//заголовок
    for (int i=0;i<sz;i++){
        vecDx<<sz_w;
    }

    vecDx<<8*wx<<8*wx<<8*wx;
    int col=0;
    double x = 5*wx;
    painter->drawLine(4*wx,y,4*wx,y+(n+1)*sz_h);//левая граница
    foreach (QString val, vecHeader) {
        painter->drawText(QRect(x,y,vecDx[col],3*wy),val,to);
        x+=vecDx[col];
        col++;
        painter->drawLine(x,y,x,y+(n+1)*sz_h);
    }

    double tw = x;//конец таблицы по x

    //дальше по всем ученикам
    font8.setBold(false);
    painter->setFont(font8);

    QTextOption to1(Qt::AlignVCenter);
    painter->drawLine(4*wx,y,tw,y);//верх
    y+=sz_h;
    int sz_lines = n;//vecData.size();
    for (int i=1;i<=sz_lines;i++){
        x = 5*wx;
        col=0;
        QVector<QString> line = vecData[start+i];
        foreach (QString val, line) {
            if (col==0){
                painter->drawText(QRect(x,y,vecDx[col],sz_h),val,to1);
            }else{
                painter->drawText(QRect(x,y,vecDx[col],sz_h),val,to);
            }

            x+=vecDx[col];
            col++;
        }
        painter->drawLine(4*wx,y,tw,y);//низ
        y+=sz_h;
    }

    painter->drawLine(4*wx,y,tw,y);//низ
}


QVector<QVector<QString> > KeyHelper::getDataForTable(ClassClass* cls, SurveyKeysClass* survs, AllKeyClass* allKeys, ResultsData* results){
    QVector<QVector<QString> > vecResults;
//    AllKeyClass* allKeys = new AllKeyClass(survs);
//    ResultsData* results = allKeys->getResultData(cls);//получили все нужные данные
    //считаем уровни

    //дальше идет таблица со всеми результатами и даже больше
    //нужна форма мо предмету чтобы узнать названия заданий

    int sz = results->mVecClsBalls.size();//всего заданий
    FormPlusData* formData = new FormPlusData;
    formData->readFormPlusData(QCoreApplication::applicationDirPath()+"/other/Form"+survs->getSurvey()+".cfpoq");
    //получили форму
//    lblNumTask3
    //ученик, список заданий

    QVector<StudentClass*> vecStd = cls->getAllStudents();
    QMap<int, double> mapTaskCompleted;//сколько каких заданий выполнено
    int total=0;

    QVector<QString> vecHeader;
    vecHeader<<"Ученик";


    for (int i=0;i<sz;i++){
        vecHeader<<formData->getValue("lblNumTask"+QString::number(i+1));
    }
    vecHeader<<"Базовый %"<<"Повышенный %"<<"Общий %";

    vecResults<<vecHeader;
    //дальше по всем ученикам

    foreach (StudentClass* stud, vecStd) {
        QVector<QString> line;
        line<<stud->getCode();
        if (stud->getMaked(survs->getSurvey())!="1"){
            //если ученик не писал то забиваем все N
            for (int i=0;i<sz;i++) {
                line<<"N";
            }
            line<<"N"<<"N"<<"N";
            vecResults<<line;
            continue;
        }
        total++;
        int k=0;
        QVector<double> vecRes = results->mVecBalls[stud->getCode()];
        foreach (double ball, vecRes) {
            QString val;
            if (ball==-1){
                val="N";
            }else{
                if (ball>0){
                    mapTaskCompleted[k]+=1.0;
                }
                val = QString::number(ball);
            }
            k++;
            line<<val;
        }
        //записываем процент выполнения заданий базового, повышенного и вообще всех
        QString val;
        val = QString::number(100*allKeys->getSumBalls(stud,0)/survs->getMaxBasic());
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        line<<val;
        val = QString::number(100*allKeys->getSumBalls(stud,1)/survs->getMaxHight());
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        line<<val;
        val = QString::number(100*allKeys->getSumBalls(stud)/survs->getMaxBall());
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        line<<val;
        vecResults<<line;
    }
    //нужен низ таблицы
    QVector<QString> vecBottom;
    vecBottom<<"Итого";
    for (int i=0;i<sz;i++){
        double res = mapTaskCompleted[i]/total;
        QString val = QString::number(res*100);
        if (val.contains(".")){
            int pos = val.indexOf(".");
            val = val.left(pos+2);
        }
        val+="%";
        vecBottom<<val;
    }
    vecBottom<<"-"<<"-"<<"-";
    vecResults<<vecBottom;
    return vecResults;
}

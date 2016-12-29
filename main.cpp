#include <QApplication>
#include "widgets/mainwindow.h"
#include "widgets/settingwidget.h"
#include "styleclass.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QApplication::setStyle(new StyleClass);
    QFont font("Times New Roman",8);
    int x = GetSystemMetrics(SM_CXSCREEN);
    if (x>3000){
            font.setPointSize(12);
    }else if (x>1900){
        font.setPointSize(11);
    }else if(x>1600){
        font.setPointSize(10);
    }else if (x>1400){
        font.setPointSize(9);
    }
    QApplication::setFont(font);
//   a.setFont(font);

    MainWindow mw;
    mw.showMaximized();

//    SettingWidget* sw = new SettingWidget(new SettingClass("open"));
//    sw->showMaximized();
    return a.exec();
}

#include "errormessage.h"

ErrorMessage::ErrorMessage()
{

}

void ErrorMessage::showError(QString text){
    QMessageBox* msb = new QMessageBox;
    msb->setWindowTitle("Ошибка!");
    msb->setText(text);
    msb->show();
}

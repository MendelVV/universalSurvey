#include "styleclass.h"

StyleClass::StyleClass():QProxyStyle()
{
}
void StyleClass::drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w)const
{
    switch(element)
    {
    case CE_HeaderEmptyArea:
        p->fillRect( opt->rect, QBrush( QColor(200,220,255) ) );
    break;
    case CE_HeaderSection://голова таблицы
    {
            //Активная секция
        if( opt->state & State_Sunken || opt->state & State_On )
        {

            QLinearGradient gradient(0, 0, 0, opt->rect.height());
            gradient.setSpread(QGradient::PadSpread);
            gradient.setColorAt(0, QColor(230,245,255));
            gradient.setColorAt(1, QColor(200,220,255));
                    //Закрашиваем секцию оранжевым цветом
            p->fillRect(opt->rect, QBrush( gradient));

            //и обрисовываем рамкой
            QPen pen;
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor( 230,230,230));
            p->setPen(pen);
                    //рисуем границы секций

            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
        }
        else//неактивная секция
        {
            //Создаем градиент для фона
            QLinearGradient gradient(0, 0, 0, opt->rect.height());
            gradient.setSpread(QGradient::PadSpread);
            gradient.setColorAt(0, QColor(240,245,255));
            gradient.setColorAt(1, QColor(210,220,255));
                    //рисуем фон
            p->fillRect(opt->rect, QBrush( gradient));

                    //создаем градиент для границ секций
            gradient.setColorAt(0, QColor(230,235,255));
            gradient.setColorAt(0.5, QColor(255,255,255));
            gradient.setColorAt(1, QColor(230,235,255));

            QPen pen;
            pen.setStyle(Qt::SolidLine);
            pen.setBrush(gradient);
            p->setPen(pen);
                    //рисуем границы секций
            p->drawLine( opt->rect.width()  + opt->rect.x() - 1,
                         opt->rect.y() + 3,
                         opt->rect.width()  + opt->rect.x() - 1,
                         opt->rect.height() + opt->rect.y() - 3 );
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor( 230,230,230));
            p->setPen(pen);
                    //рисуем границы секций
            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
        }
    }
        break;
    case CE_PushButton:{//кнопка
        const QStyleOptionButton * option = qstyleoption_cast<const QStyleOptionButton  *>(opt);
        QPen pen;
        if( opt->state & State_HasFocus && w->objectName()=="ButtonCell"){
            p->fillRect(opt->rect, QColor(229,241,251));
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor(0,120,215));
            p->setPen(pen);
            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);

            //нужно узнать размеры виджета, размеры текста, и нарисовать текст по середине

            QFontMetrics fontm (w->font());
            int count = fontm.width(option->text);

            QRect textRect = option->rect;
            int dx = (option->rect.width()-count)/2;
            textRect.setX( textRect.x() + dx );
            textRect.setY( textRect.y() + 5 );
            if(!option->text.isEmpty()){
                //Рисуем текст.
                pen.setColor(QColor( 0,0,0));
                p->setPen(pen);
                p->drawText(textRect,option->text);
            }
        }else{
            QProxyStyle::drawControl(element, opt, p, w);
        }
    }
    break;
    case CE_ComboBoxLabel:{
        QPen pen;
        if (opt->state & State_HasFocus && w->objectName()=="ComboBoxCell"){
            QProxyStyle::drawControl(element, opt, p, w);
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor(0,120,215));
            p->setPen(pen);
            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
            pen.setColor(QColor(0,0,0));
            p->setPen(pen);
        }else{
            QProxyStyle::drawControl(element, opt, p, w);
        }
    }
    break;
    case CE_CheckBox:{
        QPen pen;
        if (opt->state & State_HasFocus && w->objectName()=="CheckBoxCell"){
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor(0,120,215));
            p->setPen(pen);
            QCheckBox* chb = (QCheckBox*)w;
            QFontMetrics fntm = w->fontMetrics();
            int count = fntm.width(chb->text())+50;
            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.x()+count,opt->rect.height()-1);
            pen.setColor(QColor(0,0,0));
            p->setPen(pen);
            QProxyStyle::drawControl(element, opt, p, w);

        }else{
            QProxyStyle::drawControl(element, opt, p, w);
        }
    }
    break;
    case CE_MenuBarEmptyArea:{//меню
        QLinearGradient gradient(0, 0, 0, opt->rect.height());
        gradient.setSpread(QGradient::PadSpread);
//        gradient.setColorAt(0, QColor(255,255,240));
//        gradient.setColorAt(0, QColor(250,250,250));
//        gradient.setColorAt(1, QColor(255,185,180));
        gradient.setColorAt(0, QColor(240,245,255));
        gradient.setColorAt(1, QColor(200,220,255));
        p->fillRect(opt->rect, QBrush(gradient));
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setColor(QColor( 230,230,230));
        p->setPen(pen);
        p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
    }
        break;
    case CE_MenuBarItem:{
        const QStyleOptionMenuItem * option = qstyleoption_cast<const QStyleOptionMenuItem  *>(opt);
             QPen pen;
        if( opt->state & State_Sunken || opt->state & State_On){
            p->fillRect(opt->rect, QColor(220,230,255));
            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor( 230,230,230));
            p->setPen(pen);
            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);


        }else if( opt->state & State_HasFocus){
           p->fillRect(opt->rect, QColor(180,195,250));
           pen.setStyle(Qt::SolidLine);
           pen.setColor(QColor( 210,210,210));
           p->setPen(pen);
           p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
        }else{
            QLinearGradient gradient(0, 0, 0, opt->rect.height());
            gradient.setSpread(QGradient::PadSpread);
            gradient.setColorAt(0, QColor(230,240,255));
            gradient.setColorAt(1, QColor(200,220,255));
            p->fillRect(opt->rect, QBrush(gradient));

            pen.setStyle(Qt::SolidLine);
            pen.setColor(QColor( 230,230,230));
            p->setPen(pen);
            p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
        }
        QRect textRect = option->rect;
        textRect.setX( textRect.x() + 5 );
        textRect.setY( textRect.y() + 5 );
        if( !option->text.isEmpty() )
        {
                    //Рисуем текст.
            pen.setColor(QColor( 0,0,0));
            p->setPen(pen);
            if (option->text.indexOf("&")==0){
                p->drawText(textRect,option->text.mid(1));
            }else{
                p->drawText(textRect,option->text);
            }
        }
    }
        break;
        default:
        QProxyStyle::drawControl(element, opt, p, w);

        break;
    }
}

void StyleClass::drawComplexControl(ComplexControl control, const QStyleOptionComplex *opt, QPainter *p, const QWidget *w) const{
    QProxyStyle::drawComplexControl(control, opt, p, w);
}

void StyleClass::drawItemPixmap(QPainter *p, const QRect &rect, int align, const QPixmap &pix) const{
    QProxyStyle::drawItemPixmap(p,rect,align,pix);
}

void StyleClass::drawItemText(QPainter *p, const QRect &rect, int flags, const QPalette &pal, bool en, const QString &text, QPalette::ColorRole textRole) const{
    QProxyStyle::drawItemText(p,rect,flags,pal,en,text,textRole);
}

void StyleClass::drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w)const{
    switch(pe){
    case PE_PanelLineEdit:{
        if (w->objectName()=="LabelCell"){
            LabelCell* wgt = (LabelCell*) w;
            p->fillRect(opt->rect, wgt->getCellPlusBase()->getColor());
        }else if (w->objectName()=="LineEditCell"){
            LineEditCell* wgt = (LineEditCell*) w;
            p->fillRect(opt->rect, wgt->getCellPlusBase()->getColor());
        }else{
            QProxyStyle::drawPrimitive(pe, opt, p, w);
        }
        QPen pen;
        pen.setStyle(Qt::SolidLine);
        pen.setWidth(1);
        pen.setCapStyle(Qt::RoundCap);
        if( opt->state & State_HasFocus){
            pen.setColor(QColor(0,120,215));
        }else{
            pen.setColor(QColor(120,120,130));
        }
        p->setPen(pen);
        p->drawRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1);
    }
    break;
    case PE_FrameLineEdit:{
        if (w->objectName()=="LabelCell"){
        /*    QPen pen;
            p->fillRect(opt->rect, QColor(255,255,255));
            pen.setStyle(Qt::SolidLine);
            pen.setWidth(1);
            pen.setCapStyle(Qt::RoundCap);
            pen.setColor(QColor(120,120,130));
            p->setPen(pen);
            p->drawRoundedRect(opt->rect.x(), opt->rect.y(),opt->rect.width()-1,opt->rect.height()-1,10,10);
        */}else{
            QProxyStyle::drawPrimitive(pe, opt, p, w);
        }
    }break;
    default:
        QProxyStyle::drawPrimitive(pe, opt, p, w);
    break;
    }
}

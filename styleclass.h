#ifndef STYLECLASS_H
#define STYLECLASS_H
#include <QProxyStyle>
#include <QtWidgets>
#include <QDebug>
#include <QFontMetrics>
#include "widgetscells.h"

class StyleClass: public QProxyStyle
{
   Q_OBJECT

public:
    StyleClass();
    void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
    void drawComplexControl(ComplexControl control, const QStyleOptionComplex *option, QPainter *painter, const QWidget *widget) const;
    void drawItemPixmap(QPainter *painter, const QRect &rect, int alignment, const QPixmap &pixmap) const;
    void drawItemText(QPainter *painter, const QRect &rect, int flags, const QPalette &pal, bool enabled, const QString &text, QPalette::ColorRole textRole) const;
    void drawPrimitive(PrimitiveElement pe, const QStyleOption *opt, QPainter *p, const QWidget *w) const;
};

#endif // STYLECLASS_H

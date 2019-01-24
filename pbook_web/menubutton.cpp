#include "menubutton.h"

#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>
#include <QRgb>


MenuButton::MenuButton(QWidget *parent) : QPushButton(parent)
{

}

void MenuButton::enterEvent(QEvent *event)
{

}

void MenuButton::leaveEvent(QEvent *event)
{
    QGraphicsColorizeEffect *eEffect = new QGraphicsColorizeEffect(this);
    this->setGraphicsEffect(eEffect);
    QPropertyAnimation *paAnimation = new QPropertyAnimation(eEffect,"color");
    paAnimation->setStartValue(QColor("#474548"));
    paAnimation->setEndValue(QColor("#1f1d1e"));
    paAnimation->setDuration(100);
    paAnimation->start();
}



#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QEvent>

class MenuButton: public QPushButton
{

    Q_OBJECT

public:
    MenuButton(QWidget *parent = 0);

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
};

#endif // MENUBUTTON_H

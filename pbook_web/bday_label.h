#ifndef BDAY_LABEL_H
#define BDAY_LABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

class bday_label : public QLabel
{
    Q_OBJECT

public:
    bday_label(QWidget *parent = 0);

protected:
    void leaveEvent(QEvent *event);
};

#endif // BDAY_LABEL_H

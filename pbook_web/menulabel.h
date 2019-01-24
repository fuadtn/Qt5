#ifndef MENULABEL_H
#define MENULABEL_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMouseEvent>

#define MAINWINDOW_WIDTH        320
#define MAINWINDOW_HEIGHT       600


class MenuLabel: public QLabel
{
    Q_OBJECT

public:
    MenuLabel(QWidget *parent = 0);
    QWidget* mw;
    QPoint dPosition;
    QPoint pPosition;
    bool isMaximized;


private slots:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

};

#endif // MENULABEL_H

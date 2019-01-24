#include "menulabel.h"


#include <QRect>
#include <QApplication>
#include <QDesktopWidget>


MenuLabel::MenuLabel(QWidget *parent) : QLabel(parent)
{
    this->mw = parent;
    this->isMaximized = false;
}

void MenuLabel::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton))
    {
        this->mw->setGeometry(
            this->mw->pos().x() + (event->x() - this->dPosition.x()),
            this->mw->pos().y() + (event->y() - this->dPosition.y()),
            this->mw->width(),
            this->mw->height()
            );
        QRect desktopGeometry = QApplication::desktop()->availableGeometry();
        QPoint cursorCoord = QCursor::pos();
        if(!(cursorCoord.y() <= 1 && cursorCoord.x() <= 1 && cursorCoord.x() >= desktopGeometry.width()-1))
        {
            this->isMaximized = false;
            this->mw->setGeometry(this->mw->x(), this->mw->y(), MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
        }
    }
}

void MenuLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
            setCursor(Qt::ClosedHandCursor);
            this->dPosition.setX(event->x());
            this->dPosition.setY(event->y());
            if(!this->isMaximized)
                this->pPosition = QPoint(this->mw->x(), this->mw->y());
    }
}

void MenuLabel::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);

    QPoint cursorCoord = QCursor::pos();
    QRect desktopGeometry = QApplication::desktop()->availableGeometry();
    if(cursorCoord.y() <= 1 || cursorCoord.x() <= 1 || cursorCoord.x() >= desktopGeometry.width()-1)
    {
            int isRight = (cursorCoord.x() < desktopGeometry.width()/2 || cursorCoord.x() <= 1) ? 0 : 1;
            this->mw->setGeometry(isRight * (desktopGeometry.width()-this->mw->width()), 0, MAINWINDOW_WIDTH, desktopGeometry.height());
            this->mw->setStyleSheet("background-color: #2f2f2e; border: 1px solid #242424;");
            this->isMaximized = true;
    }
}

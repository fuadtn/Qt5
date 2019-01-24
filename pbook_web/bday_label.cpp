#include "bday_label.h"

bday_label::bday_label(QWidget *parent) : QLabel(parent)
{
}

void bday_label::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);

    QPixmap bdayPixmap(":/ico/new/bday_gray24.png");
    this->resize(bdayPixmap.width(), bdayPixmap.height());
    this->setPixmap(bdayPixmap);
}


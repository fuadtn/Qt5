#include "mainwindow.h"
#include "ui_mainwindow.h"

#ifdef _WIN32
    #include <Windows.h>
#endif


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent, Qt::Window | Qt::FramelessWindowHint),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->fillSettings(false);
    ui->editData->setPlaceholderText("Тел.: 2732-000\nE-mail: name@kandk.ru\nДолжность: [требующая обновлений]");
    ui->quitButton->setToolTip("Выход");
    ui->topButton->setToolTip("Поверх всех окон");
    ui->minButton->setToolTip("Свернуть в трей");
    ui->maxButton->setToolTip("Развернуть");
    ui->changeButton->setToolTip("Переключить список");
    ui->searchEdit->setPlaceholderText("Введите строку для поиска...");
    ui->checkBox->setGeometry(12, 82, ui->checkBox->width(), ui->checkBox->height());
    ui->checkBox->setChecked(false);
    ui->checkBox->setText("Контакты без телефонных номеров исключены");
    ui->exportToExcelBtn->setCursor(Qt::PointingHandCursor);

    this->setStyleSheet("QWidget { background-color: #2f2f2e; border: 1px solid #242424; } QToolTip {background: #ffffff;}");
    this->resize(MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
    this->isStayTop = false;
    this->isMaximized = false;
    this->isChanged = 0;

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon(":/pbook.ico"));
    trayIcon->setToolTip("Cправочник K&K");
    QMenu *trayMenu = new QMenu(this);
    this->isContained = false;

    QAction *closeMenu = new QAction(trUtf8("Выйти"), this);
    connect(closeMenu, SIGNAL(triggered()), this, SLOT(close()));
    trayMenu->addAction(closeMenu);
    trayIcon->setContextMenu(trayMenu);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    QPixmap logoPixmap(":/ico/logo.png");
    ui->logoLabel->setGeometry(ui->logoLabel->x(), ui->logoLabel->y(), logoPixmap.width(), logoPixmap.height());
    ui->logoLabel->setPixmap(logoPixmap);

    ui->changeButton->setGeometry(ui->changeButton->x(), CHANGEBTN_Y_PBOOK, ui->changeButton->width(), ui->changeButton->height());

    this->timer = new QTimer;
    connect(this->timer, SIGNAL(timeout()), this, SLOT(slotTimerEvent()));
    this->slotTimerEvent();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete trayIcon;
    this->timer->stop();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && this->isContained == true)
    {
        this->setGeometry(
            this->pos().x() + (event->x() - this->dPosition.x()),
            this->pos().y() + (event->y() - this->dPosition.y()),
            this->width(),
            this->height()
            );
        QRect desktopGeometry = QApplication::desktop()->availableGeometry();
        QPoint cursorCoord = QCursor::pos();
        if(!(cursorCoord.y() <= 1 && cursorCoord.x() <= 1 && cursorCoord.x() >= desktopGeometry.width()-1))
        {
            this->isMaximized = false;
            this->setGeometry(this->x(), this->y(), MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
        }
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton)
    {
        if((event->y() < MAINFRAME_HEIGHT))
        {
            setCursor(Qt::ClosedHandCursor);
            this->dPosition.setX(event->x());
            this->dPosition.setY(event->y());
            this->isContained = true;
            if(!this->isMaximized)
            this->pPosition = QPoint(this->x(), this->y());
        }
        else this->isContained = false;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    setCursor(Qt::ArrowCursor);

    if(this->isContained == true)
    {
        QPoint cursorCoord = QCursor::pos();
        QRect desktopGeometry = QApplication::desktop()->availableGeometry();
        if(cursorCoord.y() <= 1 || cursorCoord.x() <= 1 || cursorCoord.x() >= desktopGeometry.width()-1)
        {
            int isRight = (cursorCoord.x() < desktopGeometry.width()/2 || cursorCoord.x() <= 1) ? 0 : 1;
            this->setGeometry(isRight * (desktopGeometry.width()-this->width()), 0, MAINWINDOW_WIDTH, desktopGeometry.height());
            this->setStyleSheet("background-color: #2f2f2e; border: 1px solid #242424;");
            this->isMaximized = true;
        }
        this->isContained = false;
    }
}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(event->y() < MAINFRAME_HEIGHT)
        this->on_maxButton_clicked();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->listBrowser->setGeometry(
                LISTAREA_MARGIN,
                LISTAREA_Y,
                ui->centralWidget->width() - 2*LISTAREA_MARGIN,
                ui->centralWidget->height() - LISTAREA_Y - LISTAREA_MARGIN
                );

    ui->exportToExcelBtn->setGeometry(
                ui->exportToExcelBtn->x(),
                this->height() - ui->exportToExcelBtn->height() - 12,
                ui->exportToExcelBtn->width(),
                ui->exportToExcelBtn->height()
                );
}

void MainWindow::fillListPbook(QString toCompare)
{
    if(this->pb.contacts.isEmpty())
    {
        ui->searchEdit->setText(ERROR_STATUS_002);
        ui->searchEdit->setDisabled(true);
    }

    short illumination;
    toCompare = toCompare.toLower();
    QString PBookBrowser("<font size=4><table>");
    for(int i = 0; i < pb.contacts.size(); i++)
    {
        if(ui->checkBox->isChecked() || !this->pb.contacts.at(i, PHONEBOOK_PHONE).isEmpty())
        {
            illumination = 0x0000;
            if(!toCompare.isEmpty())
            {
                illumination |= pb.contacts.at(i, PHONEBOOK_NAME).toLower().contains(toCompare) ? 0x0001 : 0;
                illumination |= pb.contacts.at(i, PHONEBOOK_PHONE).toLower().contains(toCompare) ? 0x0010 : 0;
                illumination |= pb.contacts.at(i, PHONEBOOK_EMAIL).toLower().contains(toCompare) ? 0x0100 : 0;
                illumination |= pb.contacts.at(i, PHONEBOOK_STATE).toLower().contains(toCompare) ? 0x1000 : 0;
            }

            if(toCompare.isEmpty() || illumination)
            PBookBrowser.append(
               "<tr>"
                    "<td style=\"text-align:right;color:#" + QString((illumination & 0x0001) ? "f7a01a" : "575558") + ";\">ФИО: </td>"
                    "<td><abbr title=\"" + pb.contacts.at(i, PHONEBOOK_NAME) + "\">" + shortString(pb.contacts.at(i, PHONEBOOK_NAME)) +
                "</td></tr><tr>"
                    "<td style=\"text-align:right;color: #" + QString((illumination & 0x0010) ? "f7a01a" : "575558") + ";\">Телефон: </td>"
                    "<td>" + pb.contacts.at(i, PHONEBOOK_PHONE) +
                "</td></tr><tr>"
                    "<td style=\"text-align:right;color: #" + QString((illumination & 0x0100) ? "f7a01a" : "575558") + ";\">E-mail: </td>"
                    "<td><a style=\"color: #ffffff; text-decoration: none;\" href=\"mailto:" + pb.contacts.at(i, PHONEBOOK_EMAIL) + "\">" + shortString(pb.contacts.at(i, PHONEBOOK_EMAIL)) + "</a>"
                "</td></tr><tr>"
                    "<td style=\"text-align:right;color: #" + QString((illumination & 0x1000) ? "f7a01a" : "575558") + ";\">Должность: </td>"
                    "<td><abbr title=\"" + pb.contacts.at(i, PHONEBOOK_STATE) + "\">" + shortString(pb.contacts.at(i, PHONEBOOK_STATE)) + "</abbr></td></tr></tr><hr>"
            );
        }
    }
    PBookBrowser.append("</table></font>");
    this->setOpacityAnimation(ui->listBrowser);
    ui->listBrowser->setText(PBookBrowser);
}

void MainWindow::fillListBdays()
{
    QString BDays("<font size=4><table><tr><td style=\"color: #f7a01a;\">Сегодня, " + QDate::currentDate().toString("d.MM.yyyy") + ", день рождения отмечают:</td></tr>");
    for(int i = 0; i < this->pb.bdays_today.size(); i++)
        BDays.append("<tr><td style=\"color: #ffffff;\">" + this->pb.bdays_today.at(i,0) + "</td></tr>");
    if(this->pb.bdays_today.isEmpty())
        BDays.append("<tr><td style=\"color: #ffffff;\">&#8212;</td></tr>");
    BDays.append("<hr><tr><td style=\"color: #f7a01a;\">Завтра, " + QDate::currentDate().addDays(1).toString("d.MM.yyyy") + ", день рождения отмечают:</td></tr>");
    for(int i = 0; i < this->pb.bdays_tomorrow.size(); i++)
        BDays.append("<tr><td style=\"color: #ffffff;\">" + this->pb.bdays_tomorrow.at(i,0) + "</td></tr>");
    if(this->pb.bdays_tomorrow.isEmpty())
        BDays.append("<tr><td style=\"color: #ffffff;\">&#8212;</td></tr>");
    BDays.append("</table></font>");
    this->setOpacityAnimation(ui->listBrowser);
    ui->listBrowser->setText(BDays);
}

void MainWindow::fillSettings(bool key)
{
    if(key)
    {
        this->setOpacityAnimation(ui->listBrowser, 500, 1, 0);
        ui->listBrowser->setText("");
    }

    ui->editName->setText("");
    ui->editData->setText("");
    ui->labelSetStatus->setText("");

    ui->editName->setVisible(key);
    ui->editData->setVisible(key);
    ui->labelSet1->setVisible(key);
    ui->labelSet2->setVisible(key);
    ui->labelSetStatus->setVisible(key);
    ui->setButton->setVisible(key);
    ui->exportToExcelBtn->setVisible(key);

    if(key)
    {
        this->setOpacityAnimation(ui->labelSet1);
        this->setOpacityAnimation(ui->labelSet2);
        this->setOpacityAnimation(ui->editName);
        this->setOpacityAnimation(ui->editData);
        this->setOpacityAnimation(ui->setButton);
        this->setOpacityAnimation(ui->exportToExcelBtn);
    }
}

QString MainWindow::shortString(QString forcut)
{
    if(forcut.length() > OUTPUT_MAX_LENGTH + 2)
    {
        forcut.remove(OUTPUT_MAX_LENGTH, forcut.length() - OUTPUT_MAX_LENGTH);
        forcut.append("...");
    }
    return forcut;
}

void MainWindow::on_quitButton_clicked()
{
    this->close();
}

void MainWindow::on_topButton_clicked()
{
    if(this->isStayTop == false)
    {
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        ui->topButton->setStyleSheet("QPushButton { border-image: url(\":/ico/top_orange.png\"); }");
        this->show();
    }
    else
    {
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
        this->activateWindow();
        ui->topButton->setStyleSheet("QPushButton { border-image: url(\":/ico/top.png\"); outline: none; } "
                                     "QPushButton:hover { border-image: url(\":/ico/top50.png\"); }"
                                     );
        this->show();
    }
    this->isStayTop = this->isStayTop ? false : true;
}

void MainWindow::on_minButton_clicked()
{
#ifdef _WIN32
    HWND hwnd = (HWND) this->winId();
    ShowWindow( hwnd, SW_HIDE);
#else
    this->hide();
#endif
}

void MainWindow::on_maxButton_clicked()
{
    if(!this->isMaximized)
    {
        this->pPosition = this->pos();
        QRect desktopGeometry = QApplication::desktop()->availableGeometry();
        int isRight = this->x() < (desktopGeometry.width()-this->width())/2 ? 0 : 1;
        this->setGeometry(isRight * (desktopGeometry.width()-MAINWINDOW_WIDTH), 0, MAINWINDOW_WIDTH, desktopGeometry.height());
    }
    else
    {
        this->setGeometry(this->pPosition.x(), this->pPosition.y(), MAINWINDOW_WIDTH, MAINWINDOW_HEIGHT);
    }
    this->setStyleSheet("background-color: #2f2f2e; border: 1px solid #242424;;");
    this->isMaximized = !this->isMaximized;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason)
    {
        case QSystemTrayIcon::Trigger:
        {
#ifdef _WIN32
            HWND hwnd = (HWND) this->winId();
            if(IsWindowVisible(hwnd))
                ShowWindow( hwnd, SW_HIDE);
            else
                ShowWindow( hwnd, SW_SHOW);
#else
            if(!this->isVisible())
                this->show();
            else
                this->hide();
#endif
            this->activateWindow();
            ui->searchEdit->setFocus();
            break;
        }
        default:
            break;
     }
}

void MainWindow::on_searchEdit_textEdited(const QString &arg1)
{
    Q_UNUSED(arg1);
    this->isChanged = -1;
    this->on_changeButton_clicked();
    this->fillListPbook(ui->searchEdit->text());
}

void MainWindow::slotTimerEvent()
{
    if(!this->pb.upload() && this->pb.contacts.isEmpty())
    {
        ui->searchEdit->setStyleSheet("color: #f7a01a; padding-left: 5px;");
        ui->searchEdit->setText(ERROR_STATUS_001);
        ui->searchEdit->setDisabled(true);
    }
    else
    {
        this->fillSettings(this->isChanged==2);
        if(this->isChanged==1)
            this->fillListBdays();
        if(this->isChanged==0)
        {
            ui->searchEdit->setFocus();
            this->fillListPbook(ui->searchEdit->text());
        }
    }

    qsrand(QTime::currentTime().msec());
    const int fullTime = 86400000;
    this->timer->stop();
    this->timer->start(fullTime - QTime::currentTime().msecsSinceStartOfDay() + qrand() % 10000); // обновление в полночь
}

void MainWindow::on_checkBox_clicked()
{
    if(ui->checkBox->isChecked() == true)
        ui->checkBox->setText("Контакты без телефонных номеров включены");
    else
        ui->checkBox->setText("Контакты без телефонных номеров исключены");

    this->isChanged = -1;
    this->on_changeButton_clicked();
    this->fillListPbook(ui->searchEdit->text());
    ui->searchEdit->setFocus();
}

void MainWindow::on_changeButton_clicked() /* переключить страницу */
{
    ui->changeButton->setDisabled(true);
    int change_duration = CHANGEBTN_TM_DURATION;
    this->isChanged = (this->isChanged + 1) % 3;
    int change_offset = CHANGEBTN_Y_PBOOK;
    if(this->isChanged == 1)
        change_offset = CHANGEBTN_Y_BDAY;
    else if(this->isChanged == 2)
        change_offset = CHANGEBTN_Y_SET;

    QPropertyAnimation *animation = new QPropertyAnimation(ui->changeButton, "geometry");
    animation->setDuration(change_duration);
    animation->setEasingCurve(QEasingCurve::OutCubic);
    animation->setStartValue(QRect(ui->changeButton->x(), ui->changeButton->y(), ui->changeButton->width(), ui->changeButton->height()));
    animation->setEndValue(QRect(ui->changeButton->x(), change_offset, ui->changeButton->width(), ui->changeButton->height()));
    animation->start();

    if(this->isChanged==2)
        this->fillSettings(true);
    else if(this->isChanged==1)
        this->fillListBdays();
    else
    {
        this->fillSettings(false);
        this->fillListPbook(ui->searchEdit->text());
    }
    ui->searchEdit->setFocus();
    ui->changeButton->setDisabled(false);
}

void MainWindow::on_listBrowser_anchorClicked(const QUrl &arg1) /* открыть e-mail */
{
    QDesktopServices::openUrl(arg1);
    fillListPbook(ui->searchEdit->text());
}

void MainWindow::on_setButton_clicked() /* отправить сообщение о редактировании контактных данных */
{
    bool key = false;
    if(!ui->editName->text().isEmpty() && ui->editName->text() != " ")
    for(int i = 0; i < this->pb.contacts.size(); i++)
        if(this->pb.contacts.at(i, PHONEBOOK_NAME) == ui->editName->text())
        {
            key = true;
            break;
        }

    if(!key || ui->editData->toPlainText().isEmpty() || ui->editData->toPlainText().length() > CHANGEBTN_MESSAGE_LIMIT)
    {
        this->setOpacityAnimation(ui->labelSetStatus, 300);
        if(!key)
            ui->labelSetStatus->setText("Неверно введены ФИО...");
        else if(ui->editData->toPlainText().isEmpty())
            ui->labelSetStatus->setText("Задано пустое сообщение...");
        else
            ui->labelSetStatus->setText("Длина сообщения превышает 150 символов...");
        return;
    }

    QString Message("<a style=\"color: #f7a01a;\">ФИО: </a>" + ui->editName->text());
#ifdef _WIN32
    Message.append("<br><a style=\"color: #f7a01a;\">Имя пользователя ПК: </a>" + QString(qgetenv("USERNAME")));
    Message.append("<br><a style=\"color: #f7a01a;\">Имя компьютера: </a>" + QString(qgetenv("COMPUTERNAME")));
    Message.append("<br><a style=\"color: #f7a01a;\">Имя удаленного ПК: </a>" + QString(qgetenv("CLIENTNAME")));
#endif
    Message.append("<br><a style=\"color: #f7a01a;\">Сообщение: </a>" + ui->editData->toPlainText());

    ui->editName->setText("");
    ui->editData->setText("");

    if(this->pb.send_to_edit(Message))
    {
        this->isChanged = -1;
        this->on_changeButton_clicked();
    }
    else
        ui->labelSetStatus->setText("Произошла ошибка отправки...");
}

void MainWindow::setOpacityAnimation(QWidget *object, int duration, int start_v, int end_v)
{
    QGraphicsOpacityEffect *opacityEffect = new QGraphicsOpacityEffect;
    QPropertyAnimation *animationSettingOpacity = new QPropertyAnimation(opacityEffect, "opacity");

    animationSettingOpacity->setDuration(duration);
    animationSettingOpacity->setEasingCurve(QEasingCurve::InCubic);
    animationSettingOpacity->setStartValue(start_v);
    animationSettingOpacity->setEndValue(end_v);

    object->setGraphicsEffect(opacityEffect);
    animationSettingOpacity->start();
}

void MainWindow::on_exportToExcelBtn_clicked()
{
    QXlsx::Document xlsx;
    xlsx.setSheetName("Справочник");
    QXlsx::Format format_header;
    format_header.setBorderStyle(QXlsx::Format::BorderDashed);
    format_header.setBorderColor(QColor(200,200,200));
    format_header.setFontColor(QColor(247,160,26));
    format_header.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    xlsx.setColumn(1, 1, 45);
    xlsx.write("A1", "ФИО", format_header);
    xlsx.setColumn(2, 2, 20);
    xlsx.write("B1", "Телефон", format_header);
    xlsx.setColumn(3, 3, 30);
    xlsx.write("C1", "E-mail", format_header);
    xlsx.setColumn(4, 4, 80);
    xlsx.write("D1", "Должность", format_header);

    QXlsx::Format format_grey;
    format_grey.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format_grey.setPatternBackgroundColor(QColor(242, 242,242));
    format_grey.setBorderIndex(1);
    format_grey.setBorderStyle(QXlsx::Format::BorderDashed);
    format_grey.setBorderColor(QColor(200,200,200));

    QXlsx::Format format_white;
    format_white.setVerticalAlignment(QXlsx::Format::AlignVCenter);
    format_white.setBorderStyle(QXlsx::Format::BorderDashed);
    format_white.setBorderColor(QColor(200,200,200));

    for(int i = 0; i < this->pb.contacts.size(); i++)
    {
        xlsx.setRow(i+1, 20);
        xlsx.write("A" + QString::number(i+2), this->pb.contacts.at(i, PHONEBOOK_NAME), i%2 ? format_white : format_grey);
        xlsx.write("B" + QString::number(i+2), this->pb.contacts.at(i, PHONEBOOK_PHONE), i%2 ? format_white : format_grey);
        xlsx.write("C" + QString::number(i+2), this->pb.contacts.at(i, PHONEBOOK_EMAIL), i%2 ? format_white : format_grey);
        xlsx.write("D" + QString::number(i+2), this->pb.contacts.at(i, PHONEBOOK_STATE), i%2 ? format_white : format_grey);
    }
    xlsx.saveAs(QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/phonebook.xlsx");
    this->isChanged = -1;
    this->on_changeButton_clicked();
}

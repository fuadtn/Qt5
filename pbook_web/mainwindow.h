#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
#include <QResizeEvent>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QDesktopWidget>
#include <QDesktopServices>
#include <QList>
#include <QTimer>
#include <QDate>
#include <QNetworkInterface>
#include <QNetworkAccessManager>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QStandardPaths>

#include "xlsx/include/xlsxdocument.h"
#include "xlsx/include/xlsxformat.h"


#include "phonebook.h"

#define LISTAREA_MARGIN         10
#define LISTAREA_Y              120
#define MAINFRAME_HEIGHT        50
#define MAINWINDOW_WIDTH        320
#define MAINWINDOW_HEIGHT       600
#define OUTPUT_MAX_LENGTH       24                      // максимальное количество символов в строке + 3 точки

#define PHONEBOOK_SERVER_URL    "http://www.kikzavod.kandk.ru/php/app/db/phonebook.php/"
#define PHONEBOOK_MAIL_URL      "http://www.kikzavod.kandk.ru/php/app/db/phonebook_mail.php/"
#define CHANGEBTN_Y_PBOOK       -6
#define CHANGEBTN_Y_BDAY        21
#define CHANGEBTN_Y_SET         48
#define CHANGEBTN_TM_DURATION   300
#define CHANGEBTN_MESSAGE_LIMIT 150

#define ERROR_STATUS_001        "ОШИБКА ЗАПРОСА E001"   // ОШИБКА ДОСТУПА К СЕРВЕРУ
#define ERROR_STATUS_002        "ОШИБКА ЗАПРОСА E002"   // НЕ ПОЛУЧЕН ОТВЕТ ОТ СЕРВЕРА


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void resizeEvent(QResizeEvent *event);

    void fillListPbook(QString toCompare = "");
    void fillListBdays();
    void fillSettings(bool key);
    static inline QString shortString(QString forcut);

private slots:
    void on_topButton_clicked();
    void on_minButton_clicked();
    void on_maxButton_clicked();
    void on_quitButton_clicked();
    void on_changeButton_clicked();
    void on_checkBox_clicked();
    void on_searchEdit_textEdited(const QString &arg1);
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void slotTimerEvent();
    void on_listBrowser_anchorClicked(const QUrl &arg1);
    void on_setButton_clicked();
    void setOpacityAnimation(QWidget *object, int duration = 500, int start_v = 0, int end_v = 1);
    void on_exportToExcelBtn_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *trayIcon;
    QPoint dPosition;               // delta position
    QPoint pPosition;               // previous position

    bool isMaximized;
    bool isContained;               // point is contained in top menu
    bool isStayTop;
    short isChanged;                // changeButton is change position

    QTimer *timer;                  // timer for timerEvent
    phonebook pb;
};

#endif // MAINWINDOW_H

#ifndef FHTTPSERVER_H
#define FHTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThreadPool>
#include <QDebug>

#include "fhttpurls.h"


class FHttpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit FHttpServer(const int portNumber = 8089, const int threadCount = QThread::idealThreadCount(), QTcpServer *parent = 0);
    void incomingConnection(qintptr handle);
    void run();
    void setPortNumber(int number);
    void setThreadCount(int count);

private:
    QThreadPool threadPool;
    int portNumber;
    int threadCount;

public slots:
};

#endif // FHTTPSERVER_H

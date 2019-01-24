#ifndef FHttpUrls_H
#define FHttpUrls_H

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QMap>
#include <QJsonObject>
#include <QJsonDocument>
#include <QUrlQuery>
#include "fhttpcontrollers.h"
#include <QTime>
#include <QMutex>


class FHttpUrls;
typedef QJsonObject (FHttpControllers::*pToController)(QUrlQuery);


class FHttpUrls : public QObject, public QRunnable
{
    Q_OBJECT
public:    
    explicit FHttpUrls(qintptr socketDescriptor, QObject *parent = 0);
    void run();

private:
    qintptr socketDescriptor;
    QMap<QString, pToController> urls;
    FHttpControllers controllers;
    QMutex qInfoMutex;

public slots:
};

#endif // FHttpUrls_H

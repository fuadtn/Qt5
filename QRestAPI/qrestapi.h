#ifndef QRESTAPI_H
#define QRESTAPI_H

#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#define HTTP_200_OK 200
#define HTTP_400_BAD_REQUEST 400
#define HTTP_403_FORBIDDEN 403


class QRestAPI
{
public:
    QRestAPI();
    int Auth(const QString url, const QString username, const QString password);
    int GET(const QString url, const QJsonObject options = QJsonObject(), QJsonDocument &retval = QJsonDocument());
    int POST(const QString url, const QJsonObject options = QJsonObject(), QJsonDocument &retval = QJsonDocument());

private:
    QString token;
    QEventLoop eventLoop;
    QNetworkAccessManager *manager;
};

#endif // QRESTAPI_H

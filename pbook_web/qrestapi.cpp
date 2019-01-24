#include "qrestapi.h"

#include <QDebug>

QRestAPI::QRestAPI()
{
    this->token.clear();
    this->manager = new QNetworkAccessManager;
    QObject::connect(this->manager, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));
}

int QRestAPI::Auth(QString url, QString username, QString password)
{
    QUrlQuery postData;
    postData.addQueryItem("username", username);
    postData.addQueryItem("password", password);

    const QUrl CURL(url);
    QNetworkRequest request(CURL);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QNetworkReply *reply = this->manager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
    this->eventLoop.exec();

    int HTTP_STATUS = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if(HTTP_STATUS == HTTP_200_OK)
    {
        QJsonDocument json_doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject json_obj = json_doc.object();
        this->token = json_obj.value("token").toString();
        return HTTP_200_OK;
    }

    this->token.clear();
    return HTTP_STATUS;
}

int QRestAPI::GET(const QString url, const QJsonObject options, QJsonDocument &retval)
{
    QString full_url(url);
    if(!options.isEmpty())
    {
        full_url += "?";
        for(int i = 0; i < options.count(); i++)
            full_url += options.keys().at(i) + "=" +  options.value(options.keys().at(i)).toString() + "&";
    }
    const QUrl CURL(full_url);

    QNetworkRequest request(CURL);
    if(!this->token.isEmpty())
        request.setRawHeader("Authorization", "Token " + this->token.toUtf8());
    QNetworkReply *reply = this->manager->get(request);
    this->eventLoop.exec();

    int HTTP_STATUS = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    retval = QJsonDocument::fromJson(reply->readAll());

    this->eventLoop.exit();
    return HTTP_STATUS;
}

int QRestAPI::POST(const QString url, const QJsonObject options, QJsonDocument &retval)
{
    const QUrl CURL(url);
    QNetworkRequest request(CURL);
    if(!this->token.isEmpty())
        request.setRawHeader("Authorization", "Token " + this->token.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = this->manager->post(request, QJsonDocument(options).toJson());
    this->eventLoop.exec();

    int HTTP_STATUS = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    retval = QJsonDocument::fromJson(reply->readAll());
    this->eventLoop.exit();
    return HTTP_STATUS;
}

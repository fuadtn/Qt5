#ifndef FHTTPCONTROLLERS_H
#define FHTTPCONTROLLERS_H

#include <QObject>
#include <QUrlQuery>
#include <QDebug>
#include <QJsonObject>

#define HTTP_STATUS             "status"
#define HTTP_OK                 "200"
#define HTTP_BAD_REQUEST        "400"
#define HTTP_NOT_FOUND          "404"
#define HTTP_METHOD_NOT_ALLOWED "405"
#define HTTP_INTERNAL_ERROR     "500"
#define HTTP_ERROR_MSG          "error"
#define HTTP_METHOD_GET         "GET"

class FHttpControllers
{
public:
    QJsonObject wavInfo(QUrlQuery urlQuery);
    QJsonObject mp3ToWav(QUrlQuery urlQuery);
};

#endif // FHTTPCONTROLLERS_H

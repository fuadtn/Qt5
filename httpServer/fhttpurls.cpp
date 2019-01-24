#include "fhttpurls.h"


FHttpUrls::FHttpUrls(qintptr socketDescriptor, QObject *parent)
    : QObject(parent), socketDescriptor(socketDescriptor)
{
    this->urls.insert("/wav-info/", &FHttpControllers::wavInfo);
    this->urls.insert("/mp3-to-wav/", &FHttpControllers::mp3ToWav);
}

void FHttpUrls::run()
{

    qInfo() << "from URLS nax";
    QTime time;
    time.start();

    QTcpSocket *socket = new QTcpSocket();
    socket->setSocketDescriptor(this->socketDescriptor);
    socket->waitForReadyRead();
    QString requestHeader(socket->readAll());

    requestHeader = requestHeader.split("\r\n").at(0);
    QString httpMethod(requestHeader.split(" ").at(0));

    QJsonObject obj;
    // сервер работает только с GET-запросами
    if(httpMethod.compare(HTTP_METHOD_GET, Qt::CaseInsensitive))
    {
        obj[HTTP_ERROR_MSG] = "method not allowed";
        obj[HTTP_STATUS] = HTTP_METHOD_NOT_ALLOWED;
    }
    else
    {
        QUrl requestUrl(requestHeader.split(" ").at(1));
        if(this->urls.contains(requestUrl.path()))
            obj = (this->controllers.*urls[requestUrl.path()])(QUrlQuery(requestUrl.query()));
        else
        {
            obj[HTTP_ERROR_MSG] = "page not found";
            obj[HTTP_STATUS] = HTTP_NOT_FOUND;
        }
    }

    QString httpStatus(obj.value(HTTP_STATUS).toString());
    obj.remove(HTTP_STATUS);
    obj["exec_time"] = (double) (time.elapsed()) / 1000;

    QJsonDocument doc(obj);
    QByteArray httpResponse("HTTP/1.1");
    httpResponse.append(httpStatus);
    httpResponse.append("\r\n\r\n");
    httpResponse.append(doc.toJson(QJsonDocument::JsonFormat::Compact));
    socket->write(httpResponse);

  //  qInfoMutex.lock();
    qInfo().noquote() << "\x1b[1;32m" + httpMethod.toUpper() << httpStatus
                      << "\x1b[1;34m" + requestHeader.split(" ").at(1)
                      << "\x1b[1;33m" + QString::number(httpResponse.size()) << "bytes\x1b[0m";
 //   qInfoMutex.unlock();

    qInfo() << "urls end";
    socket->waitForBytesWritten();
    socket->disconnectFromHost();
    socket->close();
    socket->deleteLater();
}

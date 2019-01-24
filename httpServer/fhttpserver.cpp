#include "fhttpserver.h"


FHttpServer::FHttpServer(const int portNumber, const int threadCount, QTcpServer *parent)
    : QTcpServer(parent), portNumber(portNumber), threadCount(threadCount)
{
}

void FHttpServer::incomingConnection(qintptr handle)
{
    // QThreadPool самостоятельно уничтожит
    // объект-наследник класса QRunnable
    FHttpUrls *httpUrls = new FHttpUrls(handle);
    this->threadPool.start(httpUrls);
}

void FHttpServer::run()
{
    this->threadPool.setMaxThreadCount(this->threadCount);

    if(this->listen(QHostAddress::Any, this->portNumber))
        qInfo() << "\x1b[1mHTTP Server:" << this->portNumber << "|"
                << this->threadCount << "thread(s)" << "\n\x1b[32mListening...\x1b[0m";
    else
        qInfo().noquote() << "\x1b[1mHTTP Server:" << this->portNumber
                          << "\n\x1b[31mError:" << this->errorString() << "\x1b[0m";
}

void FHttpServer::setPortNumber(int number)
{
    this->portNumber = number;
}

void FHttpServer::setThreadCount(int count)
{
    this->threadCount = count;
}

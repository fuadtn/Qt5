#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include "fhttpserver.h"


int main(int argc, char *argv[])
{
    QCoreApplication application(argc, argv);
    QCoreApplication::setApplicationName("HTTP Server");
    QCoreApplication::setApplicationVersion("1.1.0");

    QCommandLineParser clParser;
    clParser.addHelpOption();
    clParser.addVersionOption();
    QCommandLineOption portOption(QStringList() << "p" << "port", "set port number (8089 by default)", "number");
    clParser.addOption(portOption);
    QCommandLineOption threadOption(QStringList() << "t" << "thread", "set count of threads", "count");
    clParser.addOption(threadOption);
    clParser.process(application);

    FHttpServer httpServer;
    if(clParser.isSet(portOption))
        httpServer.setPortNumber(clParser.value(portOption).toInt());
    if(clParser.isSet(threadOption))
        httpServer.setThreadCount(clParser.value(threadOption).toInt());
    httpServer.run();

    return application.exec();
}

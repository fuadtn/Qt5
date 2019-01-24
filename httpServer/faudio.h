#ifndef FAUDIO_H
#define FAUDIO_H

#include <QObject>
#include <QFile>


class FAudio
{
public:
    FAudio(const QString filename = QString(), const int byteCount = 0);
    bool load(const QString filename = QString(), const int byteCount = 0);
    bool isEmpty();
    QString getLastError();

protected:
    QByteArray file;
    QString lastError;
};

#endif // FAUDIO_H

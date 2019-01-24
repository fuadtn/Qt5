#include "faudio.h"


FAudio::FAudio(const QString fileName, const int byteCount)
{
    this->lastError = QString();
    this->file = QByteArray();
    if(!fileName.isEmpty())
        this->load(fileName, byteCount);
}


bool FAudio::load(const QString fileName, const int byteCount)
{
    this->lastError = QString();
    QFile audioFile(fileName);
    if(!audioFile.open(QIODevice::ReadOnly))
    {
        this->lastError = "cannot open local file";
        return false;
    }
    this->file = byteCount ? audioFile.read(byteCount) : audioFile.readAll();
    if(this->isEmpty())
    {
        this->lastError = "local file is empty";
        return false;
    }
    return true;
}

bool FAudio::isEmpty()
{
    return !this->file.length();
}

QString FAudio::getLastError()
{
    return this->lastError;
}


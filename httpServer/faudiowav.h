#ifndef AUDIOWAV_H
#define AUDIOWAV_H

#include <QObject>
#include "faudio.h"

#define AUDIO_WAV_HEADER_SIZE 44


class FAudioWAV : public FAudio
{
public:
    FAudioWAV(const QString filename = QString(), const int byteCount = 0);
    static void convertFromMP3(QString mp3FileName, QString wavFileName);

   /* т. к. количество каналов и частота дискретизации много меньше 2^32,
    * то хранить значения будем в int,
    * чтобы избежать неявного преобразования типов
    * и иметь возможность венуть -1 в качестве ошибки */
    int getChannelCount();
    int getSampleRate();
};

#endif // AUDIOWAV_H

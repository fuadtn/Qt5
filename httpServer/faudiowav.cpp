#include "faudiowav.h"


FAudioWAV::FAudioWAV(const QString filename, const int byteCount)
    : FAudio(filename, byteCount)
{
}

void FAudioWAV::convertFromMP3(QString mp3FileName, QString wavFileName)
{
    system(QString("sox -t mp3 %1 -t wav %2 2>/dev/null").arg(mp3FileName).arg(wavFileName).toStdString().c_str());
}

int FAudioWAV::getChannelCount()
{
    /* байты 22-23 (little-endian) - количество канналов */
    if(this->file.length() < AUDIO_WAV_HEADER_SIZE)
        return -1;
    int channelCount = 0;
    memcpy(&channelCount, this->file.mid(22, 2).data(), 2);
    return channelCount;
}

int FAudioWAV::getSampleRate()
{
    /* байты 24-27 (little-endian) - частота дискредитации */
    if(this->file.length() < AUDIO_WAV_HEADER_SIZE)
        return -1;
    int sampleRate = 0;
    memcpy(&sampleRate, this->file.mid(24, 4).data(), 4);
    return sampleRate;
}

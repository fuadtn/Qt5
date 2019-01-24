#include "fhttpcontrollers.h"
#include "famazons3.h"
#include "faudiowav.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QFileInfo>


QJsonObject FHttpControllers::wavInfo(QUrlQuery urlQuery)
{
    //wav-info?wavkey=test1.wav

    const QString WAVKEY("wavkey");
    const QString TEMP_FOLDER(AWS_S3_TEMP_DIRECTORY);
    const QString TEMP_FILE("temp");

    QJsonObject obj;
    FAmazonS3 s3;
    obj[HTTP_STATUS] = HTTP_OK;

    QRegularExpression re(".wav$", QRegularExpression::CaseInsensitiveOption);
    if(urlQuery.hasQueryItem(WAVKEY))
    {
        QRegularExpressionMatch match = re.match(urlQuery.queryItemValue(WAVKEY));
        if(match.hasMatch())
        {
            s3.get(TEMP_FOLDER + TEMP_FILE, urlQuery.queryItemValue(WAVKEY));
            if(s3.getLastError().isEmpty())
            {
                FAudioWAV wav(TEMP_FOLDER + TEMP_FILE, AUDIO_WAV_HEADER_SIZE);
                if(wav.isEmpty())
                {
                    obj[HTTP_STATUS] = HTTP_INTERNAL_ERROR;
                    obj[HTTP_ERROR_MSG] = wav.getLastError();
                }
                else
                {
                    obj["channel_count"] = wav.getChannelCount();
                    obj["sample_rate"] = wav.getSampleRate();
                    FAmazonS3::removeLocalFile(TEMP_FOLDER + TEMP_FILE);
                }
            }
            else
            {
                obj[HTTP_STATUS] = HTTP_INTERNAL_ERROR;
                obj[HTTP_ERROR_MSG] = s3.getLastError();
            }
        }
        else
        {
            obj[HTTP_STATUS] = HTTP_BAD_REQUEST;
            obj[HTTP_ERROR_MSG] = "invalid extension";
        }
    }
    else
    {
        obj[HTTP_STATUS] = HTTP_BAD_REQUEST;
        obj[HTTP_ERROR_MSG] = "not enough parameters";
    }
    return obj;
}

QJsonObject FHttpControllers::mp3ToWav(QUrlQuery urlQuery)
{
    //mp3-to-wav?mp3key=test2.mp3&wavkey=test2.wav

    const QString MP3KEY("mp3key");
    const QString WAVKEY("wavkey");
    const QString TEMP_FILE("temp");
    const QString TEMP_WAV_FILE("temp.wav");
    const QString TEMP_FOLDER(AWS_S3_TEMP_DIRECTORY);

    FAmazonS3 s3;
    QJsonObject obj;
    obj[HTTP_STATUS] = HTTP_OK;

    if(urlQuery.hasQueryItem(WAVKEY) && urlQuery.hasQueryItem(MP3KEY))
    {
        s3.get(TEMP_FOLDER + TEMP_FILE, urlQuery.queryItemValue(MP3KEY));
        if(s3.getLastError().isEmpty())
        {
            FAudioWAV::convertFromMP3(TEMP_FOLDER + TEMP_FILE, TEMP_FOLDER + TEMP_WAV_FILE);
            s3.put(TEMP_FOLDER + TEMP_WAV_FILE, urlQuery.queryItemValue(WAVKEY));
            if(s3.getLastError().isEmpty())
            {
                obj["file_size"] = QFileInfo(TEMP_FOLDER + TEMP_WAV_FILE).size();
                FAmazonS3::removeLocalFile(TEMP_FOLDER + TEMP_FILE);
                FAmazonS3::removeLocalFile(TEMP_FOLDER + TEMP_WAV_FILE);
            }
            else
            {
                obj[HTTP_STATUS] = HTTP_INTERNAL_ERROR;
                obj[HTTP_ERROR_MSG] = s3.getLastError();
            }
        }
        else
        {
            obj[HTTP_STATUS] = HTTP_INTERNAL_ERROR;
            obj[HTTP_ERROR_MSG] = s3.getLastError();
        }
    }
    else
    {
        obj[HTTP_STATUS] = HTTP_BAD_REQUEST;
        obj[HTTP_ERROR_MSG] = "not enough parameters";
    }
    return obj;
}

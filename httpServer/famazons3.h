#ifndef FAMAZONS3_H
#define FAMAZONS3_H

#include <QObject>
#include <QDir>

#include <aws/core/Aws.h>
#include <aws/s3/S3Client.h>

#include <aws/s3/model/Object.h>
#include <aws/s3/model/GetObjectRequest.h>
#include <aws/s3/model/PutObjectRequest.h>

#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/client/ClientConfiguration.h>

#include <fstream>
#include <iostream>

#define AWS_S3_TEMP_DIRECTORY "tmp/"


class FAmazonS3
{
public:
    FAmazonS3();
    bool get(QString fileName, QString keyName);
    bool put(QString fileName, QString keyName);
    static bool removeLocalFile(QString fileName);
    QString getLastError();

private:
    Aws::Auth::AWSCredentials creditials;
    Aws::Client::ClientConfiguration configuration;
    QString lastError;
};

#endif // FAMAZONS3_H

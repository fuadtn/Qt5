#include "famazons3.h"


const char* const S3_ACCESS_KEY_ID = "*****";
const char* const S3_ACCESS_SECRET_KEY = "*****";
const char* const S3_BUCKET_NAME = "*****";


FAmazonS3::FAmazonS3()
{
    this->lastError = QString();
    this->configuration.region = Aws::Region::US_EAST_1;
    this->creditials.SetAWSAccessKeyId(S3_ACCESS_KEY_ID);
    this->creditials.SetAWSSecretKey(S3_ACCESS_SECRET_KEY);
}

bool FAmazonS3::get(QString fileName, QString keyName)
{
    this->lastError = QString();
    bool result = false;
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::S3::S3Client s3_client(this->creditials, this->configuration);
    Aws::S3::Model::GetObjectRequest object_request;
    object_request.WithBucket(S3_BUCKET_NAME).WithKey(keyName.toStdString().c_str());

    Aws::S3::Model::GetObjectOutcome get_object_outcome = s3_client.GetObject(object_request);
    if (get_object_outcome.IsSuccess())
    {
        Aws::OFStream local_file;
        local_file.open(fileName.toStdString().c_str(), std::ios::out | std::ios::binary);

        if(local_file.is_open())
            local_file << get_object_outcome.GetResult().GetBody().rdbuf(),
            result = true;
        else
            this->lastError = "Aws::S3::Get::Error::Cannot open file or find destination path",
            result = false;
    }
    else
    {
        this->lastError = "Aws::S3::Get::Error::";
        this->lastError.append(get_object_outcome.GetError().GetMessage().c_str());
        result = false;
    }
    Aws::ShutdownAPI(options);
    return result;
}

bool FAmazonS3::put(QString fileName, QString keyName)
{
    this->lastError = QString();
    bool result;
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    Aws::S3::S3Client s3_client(this->creditials, this->configuration);
    Aws::S3::Model::PutObjectRequest object_request;
    object_request.WithBucket(S3_BUCKET_NAME).WithKey(keyName.toStdString().c_str());

    auto input_data = Aws::MakeShared<Aws::FStream>(
                "PutObjectInputStream", fileName.toStdString().c_str(),
                std::ios_base::in | std::ios_base::binary
                );
    object_request.SetBody(input_data);

    Aws::S3::Model::PutObjectOutcome put_object_outcome = s3_client.PutObject(object_request);
    if (put_object_outcome.IsSuccess())
        result = true;
    else
    {
        this->lastError = "Aws::S3::Put::Error::";
        this->lastError.append(put_object_outcome.GetError().GetMessage().c_str());
        result = false;
    }

    Aws::ShutdownAPI(options);
    return result;
}

bool FAmazonS3::removeLocalFile(QString fileName)
{
    QDir directory;
    return directory.remove(fileName);
}

QString FAmazonS3::getLastError()
{
    return this->lastError;
}

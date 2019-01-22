### Qt5 static library for HTTP requests with authorization by token.
```
    int Auth(const QString url, const QString username, const QString password);
    int GET(const QString url, const QJsonObject options = QJsonObject(), QJsonDocument &retval = QJsonDocument());
    int POST(const QString url, const QJsonObject options = QJsonObject(), QJsonDocument &retval = QJsonDocument());
```

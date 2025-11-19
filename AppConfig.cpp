#include "ApiClient.h"
#include "AppConfig.h"
#include <QNetworkRequest>
#include <QUrl>

ApiClient::ApiClient(QObject *parent)
    : QObject(parent), networkManager(new QNetworkAccessManager(this))
{
    connect(networkManager, &QNetworkAccessManager::finished,
            this, &ApiClient::handleResponse);
}

void ApiClient::sendQuery(const QString &queryText)
{
    QUrl apiUrl(AppConfig::getApiUrl());
    QNetworkRequest request(apiUrl);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject json;
    json["query"] = queryText;

    networkManager->post(request, QJsonDocument(json).toJson());
}

void ApiClient::handleResponse(QNetworkReply *reply)
{
    QString message;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument json = QJsonDocument::fromJson(data);
        if (json.isObject() && json.object().contains("response"))
            message = json.object()["response"].toString();
        else
            message = QString::fromUtf8(data);
    } else {
        message = "⚠️ Error: " + reply->errorString();
    }

    emit responseReady(message);
    reply->deleteLater();
}

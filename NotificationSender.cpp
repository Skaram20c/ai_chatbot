#include "NotificationSender.h"
#include <cstdio>
#include <qdebug.h>
#include <qlogging.h>
#include <qprocess.h>

#ifdef _WIN32
#define popen _popen
#define pclose _pclose
#endif

// Constructor
NotificationSender::NotificationSender() : webhookUrl("") {}

// Set Webhook URL
void NotificationSender::setWebhookUrl(const std::string &url)
{
    webhookUrl = url;
}

// Escape JSON safely
std::string NotificationSender::escapeJson(const std::string &s)
{
    std::string out;
    out.reserve(s.size());

    for (char c : s)
    {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }

    return out;
}

// Send email through JSON POST request
bool NotificationSender::sendEmailNotification(const std::vector<std::string> &chatHistory,
                                               const std::string &userEmail,
                                               const std::string &subject)
{
    if (webhookUrl.empty())
        return false;

    // Combine chat history into one multi-line string
    std::string combinedChat;
    for (const auto &line : chatHistory)
    {
        combinedChat += line + "\n";
    }

    // Escape for JSON safety
    std::string safeText = escapeJson(combinedChat);
    std::string safeSubject = escapeJson(subject);
    std::string safeEmail = escapeJson(userEmail);

    // Build the JSON payload
    std::string jsonPayload =
        "{"
        "\"email\":\"" + safeEmail + "\","
                      "\"subject\":\"" + safeSubject + "\","
                        "\"message\":\"" + safeText + "\""
                     "}";

    // Curl command
    std::string cmd =
        "curl -s -X POST -H \"Content-Type: application/json\" "
        "-d \"" + jsonPayload + "\" "
                        "\"" + webhookUrl + "\"";

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe) return false;

    char buffer[256];
    std::string result;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr)
        result += buffer;

    pclose(pipe);

    return true;  // You could also check for specific success response
}

// Send PDF attachment using multipart/form-data (curl)
bool NotificationSender::sendEmailWithAttachment(const std::string &filePath,
                                                 const std::string &email,
                                                 const std::string &subject,
                                                 const std::string &body)
{
    if (webhookUrl.empty()) {
        qDebug() << "Webhook URL not set";
        return false;
    }

    QStringList args;

    args << "-s" << "-X" << "POST"
         << QString::fromStdString(webhookUrl)
         << "-F" << QString("email=%1").arg(QString::fromStdString(email))
         << "-F" << QString("subject=%1").arg(QString::fromStdString(subject))
         << "-F" << QString("message=%1").arg(QString::fromStdString(body))
         << "-F" << QString("attachment=@%1;type=application/pdf").arg(QString::fromStdString(filePath));

    QProcess proc;
    proc.start("curl", args);
    proc.waitForFinished();

    int exitCode = proc.exitCode();
    QByteArray out = proc.readAllStandardOutput();
    QByteArray err = proc.readAllStandardError();

    qDebug() << "curl exitCode:" << exitCode;
    qDebug() << "curl stdout:" << out;
    qDebug() << "curl stderr:" << err;

    return (exitCode == 0);
}

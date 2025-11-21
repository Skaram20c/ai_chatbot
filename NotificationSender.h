#ifndef NOTIFICATION_SENDER_H
#define NOTIFICATION_SENDER_H

#include <string>
#include <vector>

class NotificationSender
{
public:
    NotificationSender();
    void setWebhookUrl(const std::string &url);

    // Send chat history as email
    bool sendEmailNotification(const std::vector<std::string> &chatHistory,
                               const std::string &userEmail,
                               const std::string &subject = "Chatbot Conversation Summary");

    bool sendEmailWithAttachment(const std::string &filePath,
                                 const std::string &email,
                                 const std::string &subject,
                                 const std::string &body);


private:
    std::string webhookUrl;
    std::string escapeJson(const std::string &s);
};

#endif // NOTIFICATION_SENDER_H

#ifndef NOTIFICATION_SENDER_H
#define NOTIFICATION_SENDER_H

#include <string>
#include <vector>

/**
 * @brief Handles all outbound email/webhook notifications.
 *
 * Supports:
 *   - Sending simple text email notifications
 *   - Sending emails with PDF attachments (via n8n webhook)
 *
 * Performs strict validation, JSON escaping, and safe process execution.
 */
class NotificationSender
{
public:
    NotificationSender();

    /** @brief Sets the target webhook URL (required before using any send function) */
    void setWebhookUrl(const std::string &url);

    /**
     * @brief Sends chat history as plain text inside JSON payload.
     * @return true if request executed (not necessarily delivered), false on error.
     */
    bool sendEmailNotification(const std::vector<std::string> &chatHistory,
                               const std::string &userEmail,
                               const std::string &subject = "Chatbot Conversation Summary");

    /**
     * @brief Sends chat transcript PDF via multipart/form-data attachment.
     * @return true if curl exited successfully, false if failed.
     */
    bool sendEmailWithAttachment(const std::string &filePath,
                                 const std::string &email,
                                 const std::string &subject,
                                 const std::string &body);


private:
    std::string webhookUrl;

    /** @brief Escapes text for safe JSON embedding. */
    std::string escapeJson(const std::string &s);
};

#endif // NOTIFICATION_SENDER_H

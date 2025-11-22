#pragma once

#include <QString>
#include <QVector>
#include <qpainter.h>

struct ChatMessage {
    bool isUser;            // true = user bubble, false = AI
    QString text;
};

/**
 * @brief Generates a professional PDF transcript of the chat,
 *        including header, timestamp, avatars, and chat bubbles.
 */
class ChatPDFExporter {
public:
    ChatPDFExporter();

    /**
     * @brief Exports the complete chat session to a PDF file.
     * @param filePath    Output PDF path.
     * @param studentName Name displayed in PDF header.
     * @param messages    List of chat messages in order.
     * @return true on success, false otherwise.
     */
    bool exportToPDF(const QString &filePath,
                     const QString &studentName,
                     const QVector<ChatMessage> &messages);

private:
    void drawHeader(QPainter &p);                                                       // Draw the top header
    int drawMessage(QPainter &p, const ChatMessage &msg, int &y, int pageWidth);        // Draw a single chat bubble block

    QRect wrapText(QPainter &p,                                                         // Compute wrapped text bounding box
                   const QString &text,
                   int maxWidth);
};

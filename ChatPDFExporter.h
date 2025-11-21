#pragma once

#include <QString>
#include <QVector>
#include <qpainter.h>

struct ChatMessage {
    bool isUser;
    QString text;
};

class ChatPDFExporter {
public:
    ChatPDFExporter();
    bool exportToPDF(const QString &filePath,
                     const QString &studentName,
                     const QVector<ChatMessage> &messages);

private:
    void drawHeader(QPainter &p);
    void drawMessage(QPainter &p, const ChatMessage &msg, int &y, int pageWidth);

    QRect wrapText(QPainter &p,
                   const QString &text,
                   int x,
                   int y,
                   int maxWidth);
};

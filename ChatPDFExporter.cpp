#include "ChatPDFExporter.h"
#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QImage>
#include <QDateTime>

ChatPDFExporter::ChatPDFExporter() {}

bool ChatPDFExporter::exportToPDF(const QString &filePath,
                                  const QString &studentName,
                                  const QVector<ChatMessage> &messages)
{
    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter p(&pdf);
    p.setRenderHint(QPainter::Antialiasing);

    int margin = 80;
    int y = margin;

    // ---- HEADER ----
    QFont headerFont("Segoe UI", 26, QFont::Bold);
    p.setFont(headerFont);
    p.drawText(0, y, pdf.width(), 50,
               Qt::AlignHCenter,
               "Student Chat Transcript");
    y += 60;

    QFont subFont("Segoe UI", 13);
    p.setFont(subFont);

    QString info = QString("Generated for: %1\n%2")
                       .arg(studentName)
                       .arg(QDateTime::currentDateTime().toString("MMMM dd, yyyy 'at' hh:mm AP"));

    p.drawText(0, y, pdf.width(), 60, Qt::AlignHCenter, info);
    y += 100;

    int pageWidth = pdf.width() - margin * 2;

    // ---- MESSAGES ----
    for (const auto &msg : messages)
    {
        drawMessage(p, msg, y, pageWidth);

        if (y > pdf.height() - 200) {
            pdf.newPage();
            y = margin;
        }
    }

    p.end();
    return true;
}
void ChatPDFExporter::drawMessage(QPainter &p,
                                  const ChatMessage &msg,
                                  int &y,
                                  int pageWidth)
{
    int avatarSize = 55;
    int bubbleMaxW = pageWidth * 0.70;
    int bubblePadding = 20;

    QString avatarPath = msg.isUser
                             ? ":/images/user.jpg"
                             : ":/images/ai.jpg";

    QImage avatar(avatarPath);
    QImage scaledAvatar = avatar.scaled(avatarSize, avatarSize,
                                        Qt::KeepAspectRatio,
                                        Qt::SmoothTransformation);

    int avatarX = msg.isUser ? 120 : 80;
    int bubbleX = msg.isUser ? 200 : 160;

    // Draw avatar
    p.drawImage(avatarX, y + 10, scaledAvatar);

    // Colors
    QColor bubbleColor = msg.isUser
                             ? QColor("#4A90E2")     // user bubble blue
                             : QColor("#FFF4C6");    // AI soft yellow

    QColor textColor = msg.isUser
                           ? QColor("#FFFFFF")
                           : QColor("#000000");

    p.setPen(Qt::NoPen);
    p.setBrush(bubbleColor);

    // ---- Text wrapping ----
    p.setFont(QFont("Segoe UI", 12));
    QRect textRect = wrapText(p, msg.text, bubbleX + bubblePadding,
                              y + bubblePadding, bubbleMaxW);

    QRect bubbleRect = textRect.adjusted(-bubblePadding,
                                         -bubblePadding,
                                         bubblePadding,
                                         bubblePadding);

    // Rounded bubble
    p.drawRoundedRect(bubbleRect, 22, 22);

    // Draw text on top
    p.setPen(textColor);
    p.drawText(textRect, msg.text);

    // move down for next message
    y = bubbleRect.bottom() + 35;
}

QRect ChatPDFExporter::wrapText(QPainter &p,
                                const QString &text,
                                int x,
                                int y,
                                int maxWidth)
{
    QFontMetrics fm(p.font());
    QRect rect = fm.boundingRect(x, y,
                                 maxWidth,
                                 10000, // allow long messages
                                 Qt::TextWordWrap,
                                 text);
    return rect;
}

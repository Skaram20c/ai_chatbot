#include "ChatPDFExporter.h"
#include <QPdfWriter>
#include <QPainter>
#include <QFont>
#include <QImage>
#include <QDateTime>

ChatPDFExporter::ChatPDFExporter() {}

// ---- PDF Layout Constants ----
static const int TOP_MARGIN = 60;
static const int SIDE_MARGIN = 90;
static const int HEADER_TEXT_SIZE = 100;
static const int SUBTITLE_HEIGHT = 70;
static const int MESSAGE_SPACING = 40;
static const int AVATAR_SIZE = 64;
static const int BUBBLE_PADDING = 16;
static const int BUBBLE_RADIUS = 22;

/**
 *  Export Chat → PDF
 */
bool ChatPDFExporter::exportToPDF(const QString &filePath,
                                  const QString &studentName,
                                  const QVector<ChatMessage> &messages)
{
    QPdfWriter pdf(filePath);
    pdf.setPageSize(QPageSize(QPageSize::A4));
    pdf.setResolution(300);

    QPainter paint(&pdf);
    paint.setRenderHint(QPainter::Antialiasing);

    int y = TOP_MARGIN;

    // ---- Header Section ----
    paint.setFont(QFont("Ariel", 16, QFont::Bold));
    paint.drawText(0, y, pdf.width(), HEADER_TEXT_SIZE, Qt::AlignCenter, "AI Companion Chat Script");
    y += HEADER_TEXT_SIZE;

    // NAME + TIMESTAMP
    QString subtitle = QString("Generated for: %1\n%2")
                           .arg(studentName)
                           .arg(QDateTime::currentDateTime().toString("MMMM dd, yyyy 'at' hh:mm AP"));

    paint.setFont(QFont("Ariel", 10));
    paint.drawText(0, y, pdf.width(), y , Qt::AlignCenter, subtitle);
    y += HEADER_TEXT_SIZE + SUBTITLE_HEIGHT;

    // Page width for bubble layout
    int pageWidth = pdf.width() - SIDE_MARGIN * 2;

    for (const ChatMessage &m : messages)
    {
        int bubbleHeight = drawMessage(paint, m, y, pageWidth);
        y += bubbleHeight + MESSAGE_SPACING;

        if (y > pdf.height() - 250) {
            pdf.newPage();
            y = TOP_MARGIN;
        }
    }
    paint.end();
    return true;
}

/**
 *  Draw Header section (Title + Student Name + Timestamp)
 */
int ChatPDFExporter::drawMessage(QPainter &p,
                                  const ChatMessage &msg,
                                  int &y,
                                  int pageWidth)
{
    // Instead calculate real PDF A4 width at 300dpi
    QPdfWriter *writer = static_cast<QPdfWriter*>(p.device());
    QRect pageRect = writer->pageLayout().fullRectPixels(writer->resolution());
    int pdfWidth = pageRect.width();

    int usableWidth = pdfWidth - (SIDE_MARGIN * 2) - AVATAR_SIZE - 40;

    // Load avatar
    QString avatarPath = msg.isUser ? ":/images/user.jpg" : ":/images/ai.jpg";
    QImage avatar(avatarPath);
    avatar = avatar.scaled(AVATAR_SIZE, AVATAR_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    int avatarX = msg.isUser
                      ? SIDE_MARGIN
                      : (pdfWidth - SIDE_MARGIN - AVATAR_SIZE);

    int bubbleX, maxTextWidth;

    // USER bubble = left side
    if (msg.isUser) {
        bubbleX = avatarX + AVATAR_SIZE + 20;
        maxTextWidth = usableWidth;
    }
    // AI bubble = right side
    else {
        maxTextWidth = usableWidth;
        bubbleX = pdfWidth - SIDE_MARGIN - AVATAR_SIZE - 40 - maxTextWidth;
    }

    // Colors
    QColor bubbleColor = msg.isUser ? QColor("#4A90E2") : QColor("#FFF1C6");
    QColor textColor   = msg.isUser ? Qt::white       : Qt::black;

    // Draw avatar
    p.drawImage(avatarX, y, avatar);

    QRect rawTextRect = wrapText(p, msg.text, maxTextWidth);

    // Now place it where you actually want:
    QRect textRect(
        bubbleX + BUBBLE_PADDING,
        y + BUBBLE_PADDING,
        rawTextRect.width(),
        rawTextRect.height()
        );

    // Use the calculated height of the text for the bubble
    QRect bubbleRect = textRect.adjusted(-BUBBLE_PADDING,
                                         -BUBBLE_PADDING,
                                         BUBBLE_PADDING,
                                         BUBBLE_PADDING);

    p.setBrush(bubbleColor);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(bubbleRect, BUBBLE_RADIUS, BUBBLE_RADIUS);

    p.setPen(textColor);
    p.drawText(textRect, msg.text);

    return bubbleRect.height();
}

/**
 *  Compute wrapped text bounding box
 */
QRect ChatPDFExporter::wrapText(QPainter &p,
                                const QString &text,
                                int maxWidth)
{
    QFontMetrics fm(p.font());

    // Always measure text at origin (0,0)
    QRect rect = fm.boundingRect(
        QRect(0, 0, maxWidth, INT_MAX),
        Qt::TextWordWrap,
        text
        );

    return rect;
}


#include "ChatBubble.h"
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>

/**
 * =====================================================================
 *  Constructor: Initializes text bubble, avatar, and layout orientation.
 * =====================================================================
 */
ChatBubble::ChatBubble(const QString &text, BubbleType type, const QString &avatar, QWidget *parent)
    : QWidget(parent), bubbleType(type)
{
    // Create text label
    textLabel = new QLabel(text);
    textLabel->setWordWrap(true);
    textLabel->setStyleSheet("font-size:15px;");

    // Avatar setup (safe load)
    avatarLabel = new QLabel();
    avatarLabel->setFixedSize(60, 60);
    avatarLabel->setPixmap(QPixmap(avatar).scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    QHBoxLayout *lay = new QHBoxLayout(this);
    lay->setContentsMargins(0,0,0,0);

    // Configure styles based on message type
    if (bubbleType == Incoming)
    {   // AI bubble (light background, dark text)
        textLabel->setStyleSheet("background:#f3f2fb; padding:12px; border-radius:15px; font-size:15px;");
        lay->addWidget(avatarLabel);
        lay->addWidget(textLabel, 1);
        lay->addStretch();
    }
    else
    {   // User bubble — blue gradient
        textLabel->setStyleSheet(
            "background:qlineargradient(x1:0,y1:0,x2:1,y2:0, "
            "stop:0 #0086C7, stop:1 #6ECBE4); "
            "color:white; padding:12px; border-radius:15px;"
            );
        lay->addStretch();
        lay->addWidget(textLabel, 1);
        lay->addWidget(avatarLabel);
    }
}

/**
 * =====================================================================
 *  Paint event placeholder
 *  Currently does nothing but can be extended for shadows/glow later.
 * =====================================================================
 */
void ChatBubble::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);
}

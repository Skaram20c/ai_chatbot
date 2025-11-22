#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QWidget>
#include <QLabel>

/**
 * @class ChatBubble
 * @brief UI widget representing a single chat message bubble.
 *
 * A ChatBubble consists of:
 *  - Avatar (left for incoming, right for outgoing)
 *  - Styled message bubble (gradient for outgoing, soft grey for incoming)
 *  - Automatic word-wrapping and spacing
 *
 * Responsibilities:
 *  - Render message text inside a rounded container
 *  - Display avatar consistently
 *  - Align bubble based on message type (Incoming / Outgoing)
 */
class ChatBubble : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief The type of bubble — incoming (AI) or outgoing (User)
     */
    enum BubbleType { Incoming, Outgoing };

    /**
     * @brief Constructor
     * @param text      Message text to display
     * @param type      Incoming or Outgoing bubble
     * @param avatar    Path to avatar image file
     * @param parent    Parent QWidget (chat layout)
     */
    ChatBubble(const QString &text,
               BubbleType type,
               const QString &avatar = "",
               QWidget *parent = nullptr);

private:
    BubbleType bubbleType;   ///< Type of message (incoming/outgoing)
    QLabel *textLabel;       ///< Displays the actual message text
    QLabel *avatarLabel;     ///< Displays the avatar image

protected:
    /**
     * @brief Handles custom painting for the widget.
     *        Currently empty but reserved for future effects (shadow, border, etc.)
     */
    void paintEvent(QPaintEvent *event) override;
};

#endif

#ifndef CHATBUBBLE_H
#define CHATBUBBLE_H

#include <QWidget>
#include <QLabel>

class ChatBubble : public QWidget
{
    Q_OBJECT

public:
    enum BubbleType { Incoming, Outgoing };

    ChatBubble(const QString &text,
               BubbleType type,
               const QString &avatar = "",
               QWidget *parent = nullptr);

private:
    BubbleType bubbleType;
    QLabel *textLabel;
    QLabel *avatarLabel;

protected:
    void paintEvent(QPaintEvent *event) override;
};

#endif

#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QScrollArea>
#include <qboxlayout.h>
#include <qpushbutton.h>
#include "MLFAQSystem.h"
#include "SidebarWidget.h"

struct ChatSession
{
    QString title;
    QList<QString> messages;     // Store all messages in sequence
};

class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    void setupUI();
    QWidget *buildTopBar();
    QWidget *buildSidebar();
    QWidget *buildChatArea();
    QWidget *buildInputBar();

    void clearChatArea();

private slots:
    void onBack();
    void onCloseClicked();
    void onSendMessage();

    // Chat session management
    void onNewChatClicked();
    void onChatSelected(int index);
    void sendChatToEmail(int index);

    void onRenameChat(int index);
    void onDeleteChat(int index);


private:
    // --- UI Elements ---
    SidebarWidget *sidebar;
    QWidget *topBar;

    QWidget *chatCard;
    QListWidget *list;
    QScrollArea *chatScroll;
    QWidget *chatContent;
    QVBoxLayout *chatLayout;

    QLineEdit *input;
    QPushButton *send;

    bool userNameCaptured = false;
    QString userName;

    // --- Chat State ---
    QVector<ChatSession> chatSessions;
    int currentChatIndex = -1;

    MLFAQSystem faqSystem;
};

#endif

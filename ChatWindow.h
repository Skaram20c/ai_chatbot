#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QLineEdit>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPushButton>
#include "MLFAQSystem.h"
#include "SidebarWidget.h"


struct ChatSession
{
    QString title;                 ///< Chat session title
    QList<QString> messages;       ///< Each entry: "USER: text" or "AI: text"
};

/**
 * @brief Main chat interface window for the AI Companion.
 *
 * Handles:
 *  - Chat UI (bubbles, scroll area, input bar)
 *  - ML model query integration
 *  - Chat session management (new, rename, delete, select)
 *  - Export to PDF + email sending via n8n
 *  - User name capture on first message
 */
class ChatWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    /// ---------- UI Builders ----------
    void setupUI();
    QWidget *buildTopBar();
    QWidget *buildSidebar();
    QWidget *buildChatArea();
    QWidget *buildInputBar();

    /// Clears all chat bubbles from the chat layout
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

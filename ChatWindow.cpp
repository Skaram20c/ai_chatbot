#include "ChatWindow.h"
#include "MainWindow.h"
#include "GradientLabel.h"
#include "ChatBubble.h"
#include "SidebarWidget.h"
#include "NotificationSender.h"
#include "ChatPDFExporter.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QDebug>
#include <QRegularExpression>
#include <QGraphicsDropShadowEffect>
#include <QPushButton>
#include <QLabel>
#include <QInputDialog>
#include <QScrollBar>

ChatWindow::ChatWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Chat Window");
    resize(1100, 600);

    setupUI();

    faqSystem.loadFromFile("cleaned_full_dataset.csv");
    //faqSystem.setApiUrl("http://localhost:5678/webhook-test/student_bot");
    //faqSystem.setApiUrl("http://localhost:5678/webhook-test/ai_chatbot");
    faqSystem.setApiUrl("https://clerkly-unpresumptive-yolando.ngrok-free.dev/predict");
}

ChatWindow::~ChatWindow() {}

void ChatWindow::setupUI()
{
    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    // Background same as reference
    central->setStyleSheet(
        "background:qlineargradient(x1:0,y1:0,x2:1,y2:1,"
        "stop:0 #f4f2fb, stop:1 #e4e2ec);"
        );

    // BUILD MAINFRAME (Unified sidebar+chat+topbar container)
    QWidget *mainFrame = new QWidget();
    mainFrame->setStyleSheet(
        "background:white;"
        "border-radius:25px;"
        );

    // Only one shadow around whole frame
    auto *mfShadow = new QGraphicsDropShadowEffect();
    mfShadow->setBlurRadius(35);
    mfShadow->setOffset(0, 6);
    mfShadow->setColor(QColor(0, 0, 0, 30));
    mainFrame->setGraphicsEffect(mfShadow);

    topBar = buildTopBar();

    sidebar = new SidebarWidget(this);

    connect(sidebar, &SidebarWidget::newChatRequested,
            this, &ChatWindow::onNewChatClicked);

    connect(sidebar, &SidebarWidget::chatSelected,
            this, &ChatWindow::onChatSelected);

    connect(sidebar, &SidebarWidget::sendChatToEmailRequested,
            this, &ChatWindow::sendChatToEmail);

    connect(sidebar, &SidebarWidget::renameChatRequested,
            this, &ChatWindow::onRenameChat);

    connect(sidebar, &SidebarWidget::deleteChatRequested,
            this, &ChatWindow::onDeleteChat);


    QWidget *chat = buildChatArea();

    // Layout for inside main frame
    QHBoxLayout *content = new QHBoxLayout();
    content->setSpacing(20);
    content->setContentsMargins(20, 15, 20, 20);
    content->addWidget(sidebar, 2);
    content->addWidget(chat, 5);

    QVBoxLayout *frameLayout = new QVBoxLayout(mainFrame);
    frameLayout->setSpacing(0);
    frameLayout->setContentsMargins(0, 0, 0, 0);
    frameLayout->addWidget(topBar);
    frameLayout->addLayout(content);

    // Final layout
    QVBoxLayout *mainLay = new QVBoxLayout(central);
    mainLay->setSpacing(30);
    mainLay->addWidget(mainFrame);
}

//
// TOP BAR (now flat + inside mainframe)
//
QWidget *ChatWindow::buildTopBar()
{
    QWidget *bar = new QWidget();
    bar->setFixedHeight(65);
    bar->setStyleSheet(
        "background:white;"
        "border-bottom:1px solid #ececec;"
        "border-top-left-radius:25px;"
        "border-top-right-radius:25px;"
        );

    // No individual shadows on top bar anymore (matches reference)

    QPushButton *back = new QPushButton();
    back->setIcon(QIcon(":/images/back.jpg"));
    back->setIconSize(QSize(26, 26));
    back->setStyleSheet("border:none; padding:8px;");
    connect(back, &QPushButton::clicked, this, &ChatWindow::onBack);

    GradientLabel *title = new GradientLabel();
    title->setText("Laurentian Academic AI Companion");
    title->setFont(QFont("Segoe UI", 20, QFont::Black));

    QPushButton *closeBtn = new QPushButton();
    closeBtn->setIcon(QIcon(":/images/close1.jpg"));
    closeBtn->setIconSize(QSize(20, 20));
    closeBtn->setStyleSheet("border:none; padding:8px;");
    connect(closeBtn, &QPushButton::clicked, this, &ChatWindow::onCloseClicked);

    QHBoxLayout *lay = new QHBoxLayout(bar);
    lay->setContentsMargins(15, 10, 15, 10);
    lay->addWidget(back);
    lay->addWidget(title, 1, Qt::AlignCenter);
    lay->addWidget(closeBtn);

    return bar;
}

//
// CHAT AREA (flat, clean, minimal shadow)
//
QWidget *ChatWindow::buildChatArea()
{
    QWidget *card = new QWidget();
    card->setStyleSheet(
        "background:white;"
        "border-radius:20px;"
        "border:1px solid #f0f0f5;"
        );

    chatContent = new QWidget();
    chatLayout = new QVBoxLayout(chatContent);
    chatLayout->setAlignment(Qt::AlignTop);

    // Default Welcome Message
    chatLayout->addWidget(new ChatBubble(
        "Hi, I am your Academic Companion! To Start with can I have your name please!",
        ChatBubble::Incoming,
        ":/images/ai.jpg"
        ));

    // Horizontal row full width
    QWidget *optionButtons = new QWidget();
    optionButtons->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    QHBoxLayout *optLayout = new QHBoxLayout(optionButtons);
    optLayout->setSpacing(15);
    optLayout->setContentsMargins(0, 0, 0, 0);
    optLayout->setAlignment(Qt::AlignLeft);

    QString btnStyle =
        "background:#f3f2fb;"
        "border-radius:15px;"
        "padding:10px 18px;"
        "font-size:14px;"
        "color:#4a4a4a;"
        "border:1px solid #e5e5f0;"
        "QPushButton:hover { background:#ece7ff; }";

    // Add row into chat (same alignment as incoming messages)
    chatLayout->addWidget(optionButtons, 0, Qt::AlignLeft);
    chatLayout->addWidget(optionButtons, 0, Qt::AlignLeft);

    // Scroll area
    chatScroll = new QScrollArea();
    chatScroll->setWidgetResizable(true);
    chatScroll->setWidget(chatContent);
    chatScroll->setStyleSheet("border:none; background:transparent;");

    QWidget *inputBar = buildInputBar();

    QVBoxLayout *lay = new QVBoxLayout(card);
    lay->setContentsMargins(20, 20, 20, 20);
    lay->addWidget(chatScroll, 1);
    lay->addWidget(inputBar);

    return card;
}

//
// INPUT BAR (clean and flat)
//
QWidget *ChatWindow::buildInputBar()
{
    QWidget *bar = new QWidget();
    bar->setStyleSheet(
        "background:white;"
        "border-radius:20px;"
        "border:1px solid #efefef;"
        "padding:8px;"
        );

    input = new QLineEdit();
    input->setPlaceholderText("Type a message...");
    input->setStyleSheet(
        "border:none; background:transparent; padding:10px; font-size:15px;"
        );

    connect(input, &QLineEdit::returnPressed, this, &ChatWindow::onSendMessage);

    send = new QPushButton();
    send->setIcon(QIcon(":/images/send.png"));
    send->setIconSize(QSize(22,22));
    send->setFixedSize(45,45);
    send->setStyleSheet(
        "background:#FFD237;"
        "border-radius:22px;"
        );
    connect(send, &QPushButton::clicked, this, &ChatWindow::onSendMessage);

    QHBoxLayout *lay = new QHBoxLayout(bar);
    lay->addWidget(input, 1);
    lay->addWidget(send);

    return bar;
}

void ChatWindow::onSendMessage()
{
    QString text = input->text().trimmed();
    if (text.isEmpty()) return;

    // Ensure there is an active chat session
    if (currentChatIndex < 0) {
        ChatSession session;
        session.title = "Chat " + QString::number(chatSessions.size() + 1);
        chatSessions.push_back(session);
        currentChatIndex = chatSessions.size() - 1;

        // Add to sidebar
        sidebar->addChatItem(session.title, currentChatIndex);
    }

    // USER MESSAGE BUBBLE
    chatLayout->addWidget(new ChatBubble(
        text,
        ChatBubble::Outgoing,
        ":/images/user.jpg"
        ));
    chatLayout->setAlignment(Qt::AlignTop);
    input->clear();

    // Save user message to current chat
    chatSessions[currentChatIndex].messages.append("USER: " + text);

    // ------------------------------------------------------------
    // 1. FIRST MESSAGE MUST BE THE USER'S NAME
    // ------------------------------------------------------------
    if (!userNameCaptured)
    {
        // Validate name
        QString attemptedName = text;

        // Reject empty
        if (attemptedName.trimmed().isEmpty()) {
            chatLayout->addWidget(new ChatBubble(
                "I didn't catch your name. Please tell me your name to continue",
                ChatBubble::Incoming,
                ":/images/ai.jpg"
                ));
            return;
        }

        // Reject numbers and symbols
        QRegularExpression onlyLetters("^[A-Za-z][A-Za-z\\s'-]*$");
        if (!onlyLetters.match(attemptedName).hasMatch()) {
            chatLayout->addWidget(new ChatBubble(
                "Invalid name. Please enter a valid name using letters only.",
                ChatBubble::Incoming,
                ":/images/ai.jpg"
                ));
            return;
        }

        // Passed validation — accept name!
        userName = attemptedName.trimmed();
        userNameCaptured = true;

        chatLayout->addWidget(new ChatBubble(
            "Thank you, " + userName + "! How can I assist you today?",
            ChatBubble::Incoming,
            ":/images/ai.jpg"
            ));

        // Don't send this message to ML model
        chatSessions[currentChatIndex].messages.append("AI: Thank you, " + userName + "!");

        return;
    }

    // ------------------------------------------------------------
    // 2. AFTER NAME: SEND QUERY TO ML MODEL
    // ------------------------------------------------------------
    std::string result = faqSystem.ask(text.toStdString());
    QString answer = QString::fromStdString(result);

    chatLayout->addWidget(new ChatBubble(
        answer,
        ChatBubble::Incoming,
        ":/images/ai.jpg"
        ));

    // Save AI answer
    chatSessions[currentChatIndex].messages.append("AI: " + answer);

    chatLayout->addStretch();

    chatScroll->verticalScrollBar()->setValue(
        chatScroll->verticalScrollBar()->maximum()
        );
}


void ChatWindow::onBack()
{
    MainWindow *mw = new MainWindow();
    mw->show();
    close();
}

void ChatWindow::onCloseClicked()
{
    close();
}

void ChatWindow::onNewChatClicked()
{
    ChatSession session;
    session.title = "Chat " + QString::number(chatSessions.size() + 1);

    chatSessions.push_back(session);
    currentChatIndex = chatSessions.size() - 1;

    sidebar->addChatItem(session.title, currentChatIndex);

    clearChatArea();

    // If user name already given → do NOT ask again
    if (userNameCaptured)
    {
        QString welcome = "New chat started! How can I help you today?";
        chatLayout->addWidget(new ChatBubble(
            welcome,
            ChatBubble::Incoming,
            ":/images/ai.jpg"
            ));

        chatSessions[currentChatIndex].messages.append("AI: " + welcome);
        return;
    }

    // If name not captured → ask for name again
    QString askName = "Let's get started! Can I know your name?";
    chatLayout->addWidget(new ChatBubble(
        askName,
        ChatBubble::Incoming,
        ":/images/ai.jpg"
        ));

    chatSessions[currentChatIndex].messages.append("AI: " + askName);
}

void ChatWindow::onChatSelected(int index)
{
    if (index < 0 || index >= chatSessions.size())
        return;

    currentChatIndex = index;

    clearChatArea();

    for (const QString &msg : chatSessions[index].messages)
    {
        bool isUser = msg.startsWith("USER:");
        QString content = msg.section(':', 1).trimmed();

        chatLayout->addWidget(new ChatBubble(
            content,
            isUser ? ChatBubble::Outgoing : ChatBubble::Incoming,
            isUser ? ":/images/user.jpg" : ":/images/ai.jpg"
            ));
    }

    chatScroll->verticalScrollBar()->setValue(
        chatScroll->verticalScrollBar()->maximum()
        );
}

void ChatWindow::sendChatToEmail(int index)
{
    if (index < 0 || index >= chatSessions.size())
        return;

    ChatSession &session = chatSessions[index];

    // 1. Build QVector<ChatMessage> from your saved chat
    QVector<ChatMessage> pdfMessages;

    for (const QString &m : chatSessions[currentChatIndex].messages)
    {
        bool isUser = m.startsWith("USER");
        QString cleanText = m.section(':', 1).trimmed();

        pdfMessages.append({ isUser, cleanText });
    }

    // 2. Generate PDF
    ChatPDFExporter exporter;
    QString pdfPath = "chat_transcript.pdf";

    exporter.exportToPDF(
        pdfPath,
        userName,
        pdfMessages
        );

    // 3. Notify via n8n
    NotificationSender sender;
    sender.setWebhookUrl("http://localhost:5678/webhook-test/chat-transcript");

    sender.sendEmailWithAttachment(
        pdfPath.toStdString(),
        "skaram20c@gmail.com",
        chatSessions[currentChatIndex].title.toStdString(),
        "Chat transcript attached."
        );
}



void ChatWindow::clearChatArea()
{
    QLayoutItem *child;
    while ((child = chatLayout->takeAt(0)) != nullptr)
    {
        delete child->widget();
        delete child;
    }
}

void ChatWindow::onRenameChat(int index)
{
    if (index < 0 || index >= chatSessions.size())
        return;

    bool ok = false;
    QString currentTitle = chatSessions[index].title;
    QString newTitle = QInputDialog::getText(
        this,
        "Rename Chat",
        "New chat name:",
        QLineEdit::Normal,
        currentTitle,
        &ok
        );

    newTitle = newTitle.trimmed();
    if (!ok || newTitle.isEmpty())
        return;

    chatSessions[index].title = newTitle;
    sidebar->updateChatTitle(index, newTitle);
}

void ChatWindow::onDeleteChat(int index)
{
    if (index < 0 || index >= chatSessions.size())
        return;

    chatSessions.removeAt(index);
    sidebar->removeChat(index);

    if (chatSessions.isEmpty()) {
        currentChatIndex = -1;
        clearChatArea();

        // Optional: show initial welcome message again
        chatLayout->addWidget(new ChatBubble(
            "Hi, I am your Academic Companion! To Start with can I have your name please!",
            ChatBubble::Incoming,
            ":/images/ai.jpg"
            ));
        return;
    }

    // Decide which chat to show next
    if (index >= chatSessions.size())
        index = chatSessions.size() - 1;

    currentChatIndex = index;
    onChatSelected(currentChatIndex);
}

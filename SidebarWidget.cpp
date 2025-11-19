#include "SidebarWidget.h"
#include <QVBoxLayout>
#include <QListWidgetItem>
#include <QMenu>
#include <QAction>

SidebarWidget::SidebarWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet("background:transparent;");

    // ================================================
    // MAIN SIDEBAR CONTAINER (rounded, matches chat area)
    // ================================================
    QWidget *mainContainer = new QWidget();
    mainContainer->setStyleSheet(
        "background:white;"
        "border-radius:20px;"
        "border: 1px solid #e4e4ef;"
        );

    QVBoxLayout *mainLayout = new QVBoxLayout(mainContainer);
    mainLayout->setContentsMargins(10, 12, 10, 12);
    mainLayout->setSpacing(5);


    // ================================================
    // NEW CHAT BUTTON (unchanged — matches your GUI)
    // ================================================
    newChatBtn = new QPushButton("New Chat");
    newChatBtn->setStyleSheet(
        "background:#0086C7;"
        "color:white;"
        "font-size:14px;"
        "padding:10px 14px;"
        "border-radius:20px;"
        );
    connect(newChatBtn, &QPushButton::clicked, this, &SidebarWidget::newChatRequested);



    // ================================================
    // NEW INNER CONTAINER (GROUP: Chat History & List)
    // ================================================
    QWidget *historyGroup = new QWidget();
    historyGroup->setStyleSheet(
        "background:white;"
        "border-top-left-radius:0px;"
        "border-top-right-radius:0px;"
        "border-bottom-left-radius:20px;"
        "border-bottom-right-radius:20px;"
        "border:1px solid #e4e4ef;"
        );

    QVBoxLayout *historyGroupLayout = new QVBoxLayout(historyGroup);
    historyGroupLayout->setContentsMargins(10, 10, 10, 10);
    historyGroupLayout->setSpacing(10);



    // ================================================
    // CHAT HISTORY BUTTON (your style — light blue)
    // ================================================
    chatHistoryBtn = new QPushButton("Chat History");
    chatHistoryBtn->setStyleSheet(
        "color:#0086C7;"
        "font-size:14px;"
        "padding:10px 14px;"
        "border-radius:0px;"
        );
    chatHistoryBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);



    // ================================================
    // CHAT LIST CARD (flat, no curves, touches bottom)
    // ================================================
    QWidget *chatListCard = new QWidget();
    chatListCard->setStyleSheet(
        "background:white;"
        "border-top-left-radius:0px;"
        "border-top-right-radius:0px;"
        "border-bottom-left-radius:20px;"
        "border-bottom-right-radius:20px;"
        "border:1px solid #e4e4ef;"
        );
    chatListCard->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *chatListLayout = new QVBoxLayout(chatListCard);
    chatListLayout->setContentsMargins(0, 0, 0, 0);
    chatListLayout->setSpacing(0);



    // ================================================
    // CHAT LIST (scrollable)
    // ================================================
    historyList = new QListWidget();
    historyList->setStyleSheet(
        "QListWidget { border:none; background:transparent; }"
        "QListWidget::item { padding:10px; font-size:14px; border-radius:20px; }"
        "QListWidget::item:hover { background:#F3F9FF; }"
        "QListWidget::item:selected { background:#E6F4FF; color:#0086C7; }"
        );

    connect(historyList, &QListWidget::itemClicked,
            this, &SidebarWidget::onChatClicked);

    historyList->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(historyList, &QListWidget::customContextMenuRequested,
            this, &SidebarWidget::onContextMenuRequested);

    chatListLayout->addWidget(historyList);



    // ================================================
    // ASSEMBLE INNER GROUP (History Button + List)
    // ================================================
    historyGroupLayout->addWidget(chatHistoryBtn);
    historyGroupLayout->addWidget(chatListCard, 1);   // fills space to bottom



    // ================================================
    // FINAL LAYOUT
    // ================================================
    mainLayout->addWidget(newChatBtn);
    mainLayout->addWidget(historyGroup, 1);           // inner container grows

    QVBoxLayout *rootLayout = new QVBoxLayout(this);
    rootLayout->setContentsMargins(1,1,1,1);
    rootLayout->addWidget(mainContainer);
}

// Add new chat into history list
//
void SidebarWidget::addChatItem(const QString &title, int index)
{
    QListWidgetItem *item = new QListWidgetItem(title);
    item->setData(Qt::UserRole, index);
    historyList->addItem(item);
}

//
// Update a chat title (optional)
//
void SidebarWidget::updateChatTitle(int index, const QString &title)
{
    for (int i = 0; i < historyList->count(); i++) {
        QListWidgetItem *item = historyList->item(i);
        if (item->data(Qt::UserRole).toInt() == index) {
            item->setText(title);
            return;
        }
    }
}

void SidebarWidget::clearChats()
{
    historyList->clear();
}

//
// When a chat is clicked
//
void SidebarWidget::onChatClicked(QListWidgetItem *item)
{
    int index = item->data(Qt::UserRole).toInt();
    emit chatSelected(index);
}

//
// Context menu (right-click)
//
void SidebarWidget::onContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = historyList->itemAt(pos);
    if (!item) return;

    int index = item->data(Qt::UserRole).toInt();

    QMenu menu;
    QAction *sendEmail   = menu.addAction("Send");
    QAction *renameAct   = menu.addAction("Rename");
    QAction *deleteAct   = menu.addAction("Delete");

    QAction *chosen = menu.exec(historyList->viewport()->mapToGlobal(pos));
    if (!chosen) return;

    if (chosen == renameAct) {
        emit renameChatRequested(index);      // tell ChatWindow to rename
    } else if (chosen == deleteAct) {
        emit deleteChatRequested(index);      // tell ChatWindow to delete
    } else if (chosen == sendEmail) {
        emit sendChatToEmailRequested(index);
    }
}

void SidebarWidget::removeChat(int index)
{
    // Find item with matching index
    for (int i = 0; i < historyList->count(); ++i)
    {
        QListWidgetItem *item = historyList->item(i);
        int itemIndex = item->data(Qt::UserRole).toInt();

        if (itemIndex == index) {
            delete historyList->takeItem(i);
            i--; // adjust index after removal
        } else if (itemIndex > index) {
            // Shift indices down for chats after the deleted one
            item->setData(Qt::UserRole, itemIndex - 1);
        }
    }
}


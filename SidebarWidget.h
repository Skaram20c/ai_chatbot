#ifndef SIDEBARWIDGET_H
#define SIDEBARWIDGET_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QMenu>
#include <qlabel.h>

class SidebarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SidebarWidget(QWidget *parent = nullptr);

    void addChatItem(const QString &title, int index);
    void updateChatTitle(int index, const QString &title);
    void clearChats();
    void removeChat(int index);

signals:
    void newChatRequested();
    void chatSelected(int index);
    void sendChatToEmailRequested(int index);
    void renameChatRequested(int index);
    void deleteChatRequested(int index);

private slots:
    void onChatClicked(QListWidgetItem *item);
    void onContextMenuRequested(const QPoint &pos);

private:
    QListWidget *historyList;
    QPushButton *newChatBtn;
    QPushButton *chatHistoryBtn;
    QLabel *historyLabel;

};

#endif // SIDEBARWIDGET_H

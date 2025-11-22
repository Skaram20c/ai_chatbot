#ifndef EMAILPROMPTDIALOG_H
#define EMAILPROMPTDIALOG_H

#include <QDialog>

class QLineEdit;
class QPushButton;

/**
 * A dialog used to collect user name and recipient email
 *        before sending the chat transcript PDF.
 *
 * Features:
 *  - Pre-fills user name from ChatWindow.
 *  - Allows editing the user name.
 *  - Validates email format and name format.
 *  - Shows user-friendly error messages.
 */
class EmailPromptDialog : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor for the EmailPromptDialog.
     * @param userName Pre-filled user name captured earlier by the chat.
     * @param parent   Parent widget (ChatWindow).
     */
    explicit EmailPromptDialog(const QString &userName,
                               QWidget *parent = nullptr);

    QString getUserName() const;                // Returns the validated and possibly edited user name
    QString getEmail() const;                   // Returns the validated email address.


private:
    QLineEdit *nameEdit;    // Editable username field
    QLineEdit *emailEdit;   // Recipient email field
    QPushButton *sendBtn;   // Send button
    void onSendClicked();
};

#endif // EMAILPROMPTDIALOG_H

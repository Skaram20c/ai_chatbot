#include "EmailPromptDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <qregularexpression.h>

/**
 * @brief Constructs the email prompt dialog.
 *        This dialog confirms user's name and collects a valid email address
 *        before sending the chat transcript PDF.
 */
EmailPromptDialog::EmailPromptDialog(const QString &userName,
                                     QWidget *parent)
    : QDialog(parent)
{
    // ---------- Window Settings ----------
    setWindowTitle("Send Chat Transcript");
    setFixedSize(400, 220);

    // ---------- USERNAME FIELD ----------
    QLabel *nameLabel = new QLabel("User Name:");
    nameEdit = new QLineEdit();
    nameEdit->setText(userName);   // Pre-fill with current user name

    // ---------- EMAIL FIELD ----------
    QLabel *emailLabel = new QLabel("Recipient Email:");
    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("example@email.com");

    // ---------- SEND BUTTON ----------
    sendBtn = new QPushButton("Send PDF");
    sendBtn->setStyleSheet("background:#FFD237; padding:10px; border-radius:8px;");

    // CONNECT BUTTON CLICK → VALIDATE INPUTS
    connect(sendBtn, &QPushButton::clicked, this, &EmailPromptDialog::onSendClicked);

    // ---------- LAYOUT ----------
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(nameLabel);
    layout->addWidget(nameEdit);

    layout->addWidget(emailLabel);
    layout->addWidget(emailEdit);

    layout->addSpacing(15);
    layout->addWidget(sendBtn);

    setLayout(layout);
}

/**
 * @brief Validates the fields when user presses the Send button.
 *        Shows meaningful error messages and prevents dialog closing if invalid.
 */
void EmailPromptDialog::onSendClicked()
{
    QString name = nameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();

    try
    {
        // -------- NAME VALIDATION --------
        if (name.isEmpty())
            throw std::runtime_error("Please enter your name.");

        // Allow letters, spaces, apostrophes, hyphens
        QRegularExpression nameRegex("^[A-Za-z][A-Za-z\\s'-]*$");
        if (!nameRegex.match(name).hasMatch())
            throw std::runtime_error("Name must contain letters only (A–Z), no digits or symbols.");

        // -------- EMAIL VALIDATION --------
        if (email.isEmpty())
            throw std::runtime_error("Please enter a recipient email address.");

        // Professional email regex (simple + safe)
        QRegularExpression emailRegex(
            "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$"
            );

        if (!emailRegex.match(email).hasMatch())
            throw std::runtime_error("Please enter a valid email address (example: user@example.com).");

        // Everything OK → close dialog
        accept();
    }
    catch (const std::exception &e)
    {
        QMessageBox::warning(this, "Invalid Input", e.what());
        return; // Prevent dialog from closing
    }
}

// Returns user name.
QString EmailPromptDialog::getUserName() const
{
    return nameEdit->text().trimmed();
}

// Returns validated email.
QString EmailPromptDialog::getEmail() const
{
    return emailEdit->text().trimmed();
}

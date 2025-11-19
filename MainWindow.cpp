#include "MainWindow.h"
#include "ChatWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include <QApplication>
#include <QScreen>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    backgroundLabel(new QLabel(this)),
    welcomeText(new QLabel(this)),
    letsTalkButton(new QPushButton("Let's Talk", this))
{
    setWindowTitle("Academic AI Companion");
    screenresize();

    // --- Load background image ---
    backgroundPixmap.load(":/images/mainpage.png");

    backgroundLabel->setPixmap(
        backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation)
        );
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();

    QWidget *central = new QWidget(this);
    central->setStyleSheet("background: transparent;");
    setCentralWidget(central);

    setupLayout();
    applyStyles();
}

MainWindow::~MainWindow() {}

void MainWindow::setupLayout()
{
    welcomeText->setText(
        "<div style='color:white;'>"
        "<h1 style='font-size:42px; font-weight:800; color:#F2BC50;'>Welcome to Laurentian<br>"
        "<span style='color:#0086C7;'>Academic AI Companion</span></h1>"
        "<p style='font-size:16px; color:#DDE6F3; margin-top:15px;'>"
        "</p>"
        "</div>"
        );
    welcomeText->setWordWrap(true);
    welcomeText->setAlignment(Qt::AlignCenter | Qt::AlignVCenter);

    // Shadow on button
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(25);
    shadow->setColor(QColor(0, 0, 0, 150));
    shadow->setOffset(0, 5);
    letsTalkButton->setGraphicsEffect(shadow);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    rightLayout->addWidget(welcomeText);
    rightLayout->addSpacing(40);
    rightLayout->addWidget(letsTalkButton, 0, Qt::AlignCenter);
    rightLayout->setContentsMargins(60, 120, 80, 100);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget());
    mainLayout->setContentsMargins(80, 60, 100, 60);
    mainLayout->addStretch(1);
    mainLayout->addLayout(rightLayout, 1);

    connect(letsTalkButton, &QPushButton::clicked, this, &MainWindow::onLetsTalkClicked);
}

void MainWindow::applyStyles()
{
    letsTalkButton->setFixedSize(220, 60);
    letsTalkButton->setStyleSheet(
        "QPushButton {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #FFD700, stop:1 #F07B00);"
        "color: #0086C7; font-size: 18px; font-weight: bold; border-radius: 30px;"
        "border: none; padding: 10px 20px;"
        "}"
        "QPushButton:hover {"
        "background: qlineargradient(x1:0,y1:0,x2:1,y2:0, stop:0 #F07B00, stop:1 #FFD700);"
        "}"
        );
}


void MainWindow::onLetsTalkClicked()
{
    ChatWindow *chat = new ChatWindow();
    chat->show();
    this->close();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    backgroundLabel->setGeometry(0, 0, width(), height());
}

void MainWindow::screenresize(){
    // --- Scale window to 80% of screen, keep title bar ---
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->availableGeometry();
    int w = static_cast<int>(screenGeometry.width() * 0.8);
    int h = static_cast<int>(screenGeometry.height() * 0.8);
    resize(w, h);
    move((screenGeometry.width() - w) / 2, (screenGeometry.height() - h) / 2);
}

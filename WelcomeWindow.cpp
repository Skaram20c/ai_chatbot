#include "WelcomeWindow.h"
#include <QPalette>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    backgroundLabel(new QLabel(this))
{
    // Window setup
    setWindowTitle("Academic AI Assistant");
    resize(1200, 800);

    // Load background from resources (we’ll add to .qrc next step)
    backgroundPixmap.load("aibot.jpg");

    // Label setup for background
    backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setGeometry(0, 0, width(), height());
    backgroundLabel->lower();  // keep behind future widgets

    // Central widget placeholder for now
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
}

MainWindow::~MainWindow() {}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    backgroundLabel->setPixmap(backgroundPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation));
    backgroundLabel->setGeometry(0, 0, width(), height());
}

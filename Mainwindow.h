#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QResizeEvent>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void screenresize();

private slots:
    void onLetsTalkClicked();

private:
    QLabel *backgroundLabel;
    QLabel *welcomeText;
    QPushButton *letsTalkButton;
    QPixmap backgroundPixmap;

    void setupLayout();
    void applyStyles();
};

#endif // MAINWINDOW_H

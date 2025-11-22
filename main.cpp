#include "MainWindow.h"
#include <QApplication>

/**
 * @brief Entry point of the Academic AI Companion application.
 *
 * Responsibilities:
 *   - Initialize the Qt application environment
 *   - Create and show the main window
 *   - Handle graceful shutdown
 *
 * Notes:
 *   - QApplication must be created before any QWidget
 *   - Always use app.exec() to start the Qt event loop
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);           // Initialize QT Application

    MainWindow w;                           // Create MainWinodw Object
    w.show();
    return app.exec();                      // Enter Qt event loop
}

#include <QApplication>
#include "server.h"
#include "mainwindow.h"

MainWindow *g_mainWindow = nullptr;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    g_mainWindow = new MainWindow();
    g_mainWindow->show();

    Server server;
    return app.exec();
}

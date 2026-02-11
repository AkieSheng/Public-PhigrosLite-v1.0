#include <QApplication>
#include "Application.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("PhigrosLite");
    app.setApplicationVersion("1.0.0");
    app.setOrganizationName("Rhythm Game Team");

    Application gameApp;

    if (!gameApp.initialize()) {
        qDebug() << "Failed to initialize application!";
        return -1;
    }
    
    gameApp.show();
    
    return app.exec();
}
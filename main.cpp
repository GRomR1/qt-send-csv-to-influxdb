#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include "serverconfigures.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings( QCoreApplication::applicationDirPath()+"/settings_new.conf", QSettings::IniFormat );

    settings.beginGroup( "ServerConfigures" );
    ServerConfigures sc;
    sc.ip = settings.value( "ip", "" ).toString();
    sc.dbname = settings.value( "dbname", "" ).toString();
    sc.user = settings.value( "user", "" ).toString();
    sc.password = settings.value( "password", "" ).toString();
    settings.endGroup();

    MainWindow w(nullptr,sc);
    w.show();

    return a.exec();
}

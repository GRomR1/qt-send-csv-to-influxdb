#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings settings( ":/settings/settings.conf", QSettings::IniFormat );

//    settings.beginGroup( "ServerConfigures" );
//    settings.setValue( "ip", "52.32.188.215" );
//    settings.endGroup();

    settings.beginGroup( "ServerConfigures" );
    QString ip = settings.value( "ip", "" ).toString();
    settings.endGroup();

    MainWindow w(nullptr,ip);
    w.show();

    return a.exec();
}

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCharts/QtCharts>
#include <QMainWindow>
#include <QMultiMap>
#include <QMap>
#include <QString>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrlQuery>
#include <QProcess>
#include <QFileDialog>
#include <QTextCodec>
#include <QChart>
#include <QDir>
#include <QTimeZone>

#include "serverconfigures.h"

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0, ServerConfigures ip = ServerConfigures());
    ~MainWindow();

signals:
    void toDebug(QString s);

private slots:
    void on_pushButtonLoad_clicked();
    void on_pushButtonDraw_clicked();
    void on_lineEditFilePath_returnPressed();
    void on_pushButtonSend_clicked();

    void debug(QString str);

    void replyFinish(QNetworkReply *reply);
    void showChart();
    void readFile();
    void loadFile();
    void sendData();
    void parseRawData();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nwam;
    QMap<QDateTime, QVariant> _data;
    QList<QStringList> _rawData;
    QStringList _head;
    ServerConfigures _sc;
};


#endif // MAINWINDOW_H

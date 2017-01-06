#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCharts/QtCharts>
#include <QMainWindow>
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

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void toDebug(QString s);

private slots:
    void on_pushButtonLoad_clicked();
    void replyFinish(QNetworkReply *reply);

    void on_pushButtonSend_clicked();
    void debug(QString str);

    void showChart();
    void readFile();

    void on_pushButtonDraw_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *nwam;
    QMap<QDateTime, uint> _data;
};


#endif // MAINWINDOW_H

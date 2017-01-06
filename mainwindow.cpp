#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    nwam = new QNetworkAccessManager;
    connect(nwam, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinish(QNetworkReply*)));
    connect(this, SIGNAL(toDebug(QString)), SLOT(debug(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonLoad_clicked()
{
    QString str =
            QFileDialog::getOpenFileName(0,
                                         tr("Open CSV File with data"),
                                         "",
                                         "*.csv"
                                         );
    if (!str.isEmpty()) {
        ui->lineEditFilePath->setText( str );
    }
}

void MainWindow::replyFinish(QNetworkReply *reply)
{
    QString answer = QString::fromUtf8(reply->readAll());
//    Q_UNUSED(answer)
    QNetworkReply::NetworkError ne = reply->error();
    qDebug() <<  ((ne == QNetworkReply::NoError)? "good" : "error");
    qDebug() << "answer" << answer;

}

void MainWindow::on_pushButtonSend_clicked()
{
    QUrlQuery query;
    query.addQueryItem("precision","s");
    query.addQueryItem("db","gridexp");
    query.addQueryItem("u","writer");
    query.addQueryItem("p","6jOuk7YPnL");
    QString   apiUrl("http://"+ui->lineEditServerIp->text()+":8086/write");
    QUrl url(apiUrl);
    url.setQuery(query);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("Content-Transfer-Encoding","binary");

    QString filename = ui->lineEditFilePath->text();
    QString measurement = QFileInfo(filename).baseName();

    readFile();

    QStringList requestList;
    QMap<QDateTime, uint>::iterator it = _data.begin();
    for (;it != _data.end(); ++it) {
        QString requestString =
                QString("%1 value=%2i %3")
                .arg(measurement)
                .arg(it.value())
                .arg(it.key().toTime_t());
        requestList << requestString;
    }
    if(!_data.isEmpty())
        nwam->post( request,
                QString(requestList.join('\n')).toUtf8() );

    _data.clear();

}

void MainWindow::debug(QString str)
{
    ui->statusBar->showMessage(str, 3000);
}


void MainWindow::showChart()
{
    readFile();
    if(_data.isEmpty())
        return;

    //prepare data
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    QMap<QDateTime, uint>::iterator it = _data.begin();
    for (;it != _data.end(); ++it) {
        series->append(it.key().toMSecsSinceEpoch(), it.value());
    }

    //add data series ti the chart
    chart->addSeries(series);

    //hide legend if needed
    chart->legend()->hide();

    //specify legend position
    chart->legend()->setAlignment(Qt::AlignBottom);

    //set chart title
    chart->setTitle(ui->lineEditFilePath->text());

    //add axis to the chart
    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("hh:mm dd.MM.yy");
    axisX->setTitleText("Date");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Value");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    //create new view
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    _data.clear();
    //plase it in this widget
    chartView->resize(600, 200);
    chartView->move(this->pos());
    chartView->setWindowTitle(ui->lineEditFilePath->text());
    chartView->show();
}

void MainWindow::readFile()
{
    QString filename = ui->lineEditFilePath->text();
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit toDebug(QString("Ошибка открытия файла '%1' для чтения").arg(filename));
        return;
    }
    QTextStream in(&file);
    QTextCodec *defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    if (defaultTextCodec)
        in.setCodec(defaultTextCodec);
    int nRow=0;
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(!line.isEmpty())
        {
            if(nRow==0) {
//                qDebug().noquote() << "H:" << line;
                nRow++;
                continue;
            }
            QStringList row = line.split(';');
            //Time Format is 10.06.2014 14:30:00
            _data.insert(QDateTime::fromString(row.at(0), "dd.MM.yyyy hh:mm:ss"), QString(row.at(2)).toInt());
            nRow++;
        }
//        if(nRow >= 10)
//            break;
    }
    file.close();


}

void MainWindow::on_pushButtonDraw_clicked()
{
    showChart();
}

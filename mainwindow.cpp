#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qtcsv/abstractdata.h>
#include <qtcsv/reader.h>

MainWindow::MainWindow(QWidget *parent, ServerConfigures sc) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _sc(sc)
{
    ui->setupUi(this);
    ui->lineEditServerIp->setText(sc.ip);

    QList<QByteArray> tzones = QTimeZone::availableTimeZoneIds();
    foreach (QByteArray ba, tzones) {
        if(ba.contains("UTC")){
            ui->_comboBoxTimeZones->addItem(ba);
        }
    }
    ui->_comboBoxTimeZones->setCurrentText("UTC+05:00");

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
                                         QDir::currentPath(),
                                         "*.csv"
                                         );
    if (!str.isEmpty()) {
        ui->lineEditFilePath->setText( str );
        QDir::setCurrent(QFileInfo(str).absoluteDir().absolutePath());
        qDebug() << "QDir::current" << QDir::currentPath();
        loadFile();
    }
}

void MainWindow::replyFinish(QNetworkReply *reply)
{
    QString answer = QString::fromUtf8(reply->readAll());
    QNetworkReply::NetworkError ne = reply->error();
    emit toDebug((ne == QNetworkReply::NoError)? "Success!" : "Error!");
    qDebug() << "answer" << answer;

}

void MainWindow::on_pushButtonSend_clicked()
{
    sendData();
}

void MainWindow::debug(QString str)
{
    ui->statusBar->showMessage(str, 3000);
}


void MainWindow::showChart()
{
    parseRawData();
    if(_data.isEmpty())
        return;

    //prepare data
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();

    QMap<QDateTime, QVariant>::iterator it = _data.begin();
    for (;it != _data.end(); ++it) {
        series->append(it.key().toMSecsSinceEpoch(), it.value().toInt());
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
    ui->tableWidget->clear();
    QString filename = ui->lineEditFilePath->text();
    _rawData = QtCSV::Reader::readToList(filename.toLower(),
                                                        ";",
                                                        "\"",
                                                        QTextCodec::codecForName("Windows-1251"));

//    foreach (QStringList ss, _rawData) {
//        qDebug() << "_rawData" << ss.join(';');
//    }
//    qDebug() << "_rawData size" << _rawData.size();

    _head = _rawData.takeFirst();
    ui->tableWidget->setColumnCount(_head.size());
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels(_head);
    ui->lineEditTimeColumn->setText("1");
    ui->lineEditTimeFormat->setText("dd.MM.yyyy hh:mm");
    ui->lineEditDataColumn->setText("2");
    ui->lineEditDataName->setText(_head.at(1));
    ui->lineEditMeasurement->setText(QFileInfo(filename).baseName().remove(".csv",Qt::CaseInsensitive));

    uint nRow = 0;
    foreach (QStringList row, _rawData) {
        ui->tableWidget->insertRow(ui->tableWidget->rowCount());
        uint nCol = 0;
        foreach (QString item, row) {
            ui->tableWidget->setItem(nRow, nCol, new QTableWidgetItem(item));
            ++nCol;
        }
        ++nRow;
    }
}

void MainWindow::loadFile()
{
    readFile();
}

void MainWindow::sendData()
{
    parseRawData();
     if(_data.isEmpty())
         return;

    QUrlQuery query;
    query.addQueryItem("precision","s");
    query.addQueryItem("db",_sc.dbname);
    query.addQueryItem("u",_sc.user);
    query.addQueryItem("p",_sc.password);
    QString   apiUrl("http://"+ui->lineEditServerIp->text()+":8086/write");
    QUrl url(apiUrl);
    url.setQuery(query);

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    request.setRawHeader("Content-Transfer-Encoding","binary");

    QString measurement = ui->lineEditMeasurement->text().trimmed();
    QString fieldname = ui->lineEditDataName->text().trimmed();
    QString tagname = ui->lineEditTagName->text().trimmed();
    QString tagvalue = ui->lineEditTagValue->text().trimmed();

    measurement.remove(',');
    fieldname.remove(',');
    tagname.remove(',');
    tagvalue.remove(',');

    measurement.replace(' ', '_');
    fieldname.replace(' ', '_');
    tagname.replace(' ', '_');
    tagvalue.replace(' ', '_');

    int countErrors=0;
    QStringList requestList;
    QMap<QDateTime, QVariant>::iterator it = _data.begin();
    for (;it != _data.end(); ++it) {
//        qDebug() << "requestList" << it.key().toString() << it.value().toString();
        if(it.key().isValid())
        {
            QString requestString;
            if(tagname.isEmpty() || tagvalue.isEmpty())
                requestString =
                        QString("%1 %2=%3 %4")
                        .arg(measurement)
                        .arg(fieldname)
                        .arg(it.value().toString().replace(',', '.'))
                        .arg(it.key().toTime_t());
            else
                requestString =
                        QString("%1,%2=%3 %4=%5 %6")
                        .arg(measurement)
                        .arg(tagname)
                        .arg(tagvalue)
                        .arg(fieldname)
                        .arg(it.value().toString().replace(',', '.'))
                        .arg(it.key().toTime_t());
            requestList << requestString;
        }
        else
        {
            ++countErrors;
        }
    }

//    qDebug() << "requestList size" << requestList.size();
    emit toDebug(QString("Count invalid time: %1").arg(QString::number(countErrors)));
    if(!_data.isEmpty())
        nwam->post(request,
                   QString(requestList.join('\n')).toUtf8());
}

void MainWindow::parseRawData()
{
    _data.clear();

    int colData = ui->lineEditDataColumn->text().toInt()-1;
    int colTime = ui->lineEditTimeColumn->text().toInt()-1;
    QString format = ui->lineEditTimeFormat->text();
//    qDebug() << "_rawData.size()" << _rawData.size();
    for(int i=0; i<_rawData.size(); ++i)
    {

        QDateTime dt = QDateTime::fromString(_rawData.at(i).at(colTime), format);
        dt.setTimeZone(QTimeZone(ui->_comboBoxTimeZones->currentText().toLocal8Bit()));
//        qDebug() << i
//                 << _rawData.at(i).at(colTime)
//                 << dt.toString()
//                 << dt.toUTC()
//                 << _rawData.at(i).at(colData);
        _data.insert(
                    dt,
                    _rawData.at(i).at(colData)
                    );
    }
}

void MainWindow::on_pushButtonDraw_clicked()
{
    showChart();
}

void MainWindow::on_lineEditFilePath_returnPressed()
{
    loadFile();
}

#ifndef CSVREADER_H
#define CSVREADER_H

#include <QObject>
#include <QString>
#include <QFile>
#include <QTextStream>

class CsvReader : public QObject
{
    Q_OBJECT
public:
    explicit CsvReader(QObject *parent = 0);
    void setFileName(QString fname = QString());

signals:
    void fileReaded(QVariant);
    void error(QString, QString);

public slots:
    void read();

private:
    QString _filename;
    QList<QVariant> _filedata;
};

#endif // CSVREADER_H

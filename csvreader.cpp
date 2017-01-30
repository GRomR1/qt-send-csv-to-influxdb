#include "csvreader.h"

CsvReader::CsvReader(QObject *parent) : QObject(parent)
{

}

void CsvReader::setFileName(QString fname)
{
    _filename = fname;
}

void CsvReader::read()
{
    QFile file(_filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        emit error(this->objectName(),
                   QString("Ошибка открытия файла '%1' для чтения").arg(_filename));
        return;
    }
    QTextStream in(&file);
    QTextCodec *defaultTextCodec = QTextCodec::codecForName("Windows-1251");
    if (defaultTextCodec)
        in.setCodec(defaultTextCodec);
    while (!in.atEnd())
    {
        QString line = in.readLine();
        if(!line.isEmpty())
        {
            QStringList row = line.split(';');
            //Time Format is 10.06.2014 14:30:00

            nRow++;
        }
    }
    file.close();



}

void CsvReader::on_action_Open_triggered()
{
    QFile file (fileName);
    if (file.open(QIODevice::ReadOnly)) {
        QString data = file.readAll();
        data.remove( QRegExp("\r") ); //remove all ocurrences of CR (Carriage Return)
        QString temp;
        QChar character;
        QTextStream textStream(&data);
        while (!textStream.atEnd()) {
            textStream >> character;
            if (character == ',') {
                checkString(temp, character);
            } else if (character == '\n') {
                checkString(temp, character);
            } else if (textStream.atEnd()) {
                temp.append(character);
                checkString(temp);
            } else {
                temp.append(character);
            }
        }
    }
}

void CsvReader::checkString(QString &temp, QChar character)
{
    if(temp.count("\"")%2 == 0) {
        //if (temp.size() == 0 && character != ',') //problem with line endings
        //    return;
        if (temp.startsWith( QChar('\"')) && temp.endsWith( QChar('\"') ) ) {
             temp.remove( QRegExp("^\"") );
             temp.remove( QRegExp("\"$") );
        }
        //FIXME: will possibly fail if there are 4 or more reapeating double quotes
        temp.replace("\"\"", "\"");
        QStandardItem *item = new QStandardItem(temp);
        standardItemList.append(item);
        if (character != QChar(',')) {
            model->appendRow(standardItemList);
            standardItemList.clear();
        }
        temp.clear();
    } else {
        temp.append(character);
    }
}


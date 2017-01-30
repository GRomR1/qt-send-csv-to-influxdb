#ifndef SERVERCONFIGURES_H
#define SERVERCONFIGURES_H

#include <QString>

struct ServerConfigures
{
public:
    ServerConfigures();
    QString ip;
    QString dbname;
    QString user;
    QString password;
};

#endif // SERVERCONFIGURES_H

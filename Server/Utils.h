#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <QString>
#include <QFile>
#include <QTextStream>

class Utils
{
public:
    Utils();
    static void printConsole(const QString &);
    static void writeLog(QFile *, const QString &);
    static const char *convertQString(const QString &);
};

#endif // UTILS_H

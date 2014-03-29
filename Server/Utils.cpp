#include "Utils.h"

Utils::Utils()
{
}

void Utils::printConsole(const QString &string)
{
    std::cout << string.toStdString() << std::endl;
}

void Utils::writeLog(QFile *log , const QString &string)
{
    QTextStream out(log);

    out << string;
}

const char *Utils::convertQString(const QString &string)
{
    return string.toStdString().c_str();
}

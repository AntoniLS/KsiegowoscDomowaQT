#include "devcontroltools.h"
#include <QDebug>
DevControlTools::DevControlTools()
{

}

void DevControlTools::consoleLog(const char *message)
{
    qDebug() <<"DEVCT LOG: "<< message;
}



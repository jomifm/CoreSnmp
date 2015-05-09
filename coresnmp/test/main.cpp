#include <QString>
#include <QCoreApplication>
#include <QtCore/QFile>

#include "snmptest.h"
#include "utils/utilsdefinitions.h"
#include "utils/logger/utilslogger.h"

int main(int argc, char *argv[])
{
    LogFuncionBegin;

    //qInstallMessageHandler(CoreSimLogger::MyLogger);
    LogInfo() << "Starting test application CoreSnmp";

    CleanExit cleanExit;
    MyApplication a(argc, argv);

    try
    {
        //Prueba la funcionalidad de base de datos
        LogInfo() << "Executing program test";

        SnmpTest snmPTest;
        snmPTest.init();

        LogInfo() << "Application starts and running.";
    	a.exec();
        LogInfo() << "Stopping application.";
    }
    catch (std::exception &e)
    {
        qCritical() << e.what();
    }

    //CoreSnmp::shutdownSnmp();
    LogInfo() << "CoreSnmp test application finished";

    return 0;
}



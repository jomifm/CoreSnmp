#include "test/snmptest.h"
#include "utils/utilsdefinitions.h"
#include "utils/logger/utilslogger.h"

#include <QString>
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    LogFuncionBegin;

    qInstallMessageHandler(CoreSnmpLogger::logger);
    LogInfo << "Starting test application CoreSnmp";

    //Utilities in application signals
    CleanExit cleanExit;
    MyApplication a(argc, argv);

    try
    {
        LogInfo << "Executing program test";

        //Instance of test program
        SnmpTest snmPTest;

        //Test program init with his own thread
        snmPTest.init();

        LogInfo << "Application starts and running.";
    	a.exec();
        LogInfo << "Stopping application.";
    }
    catch (std::exception &e)
    {
    	//Show catched exception
    	LogError << e.what();
    }
    catch (...)
    {
    	//Show catched exception
        LogError << "Fatal error in main application";
    }

    //CoreSnmp::shutdownSnmp();
    LogInfo << "CoreSnmp test application finished";

    return 0;
}



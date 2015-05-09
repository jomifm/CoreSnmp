#include "configuration/parameters.h"

#include <QSettings>
#include <QVariant>
#include <QDebug>

//Ficheros de Configuracion
const QString SdgsFileParameters = QString("cfg/qsnmp.cnf");

Parameters *Parameters::snmpparams_;

Parameters::Parameters(QObject *parent) :
    QObject(parent)
{
    //Inicializacion de Variables
    strGeneralVersion = "";
    strLoggerLevel = "INFO";
    strLoggerFile = "";

    //Inicializacion de parametros
    intSnmpPortNms = 0;
    intSnmpPortAgent = 0;
    intSnmpTrapPortNms = 0;
    intSnmpTrapPortAgent = 0;
    strSnmpTrapAddressNms.clear();
    strSnmpTrapAddressAgent = "";

    //Lectura de Configuracion
    readParameters();

    //Reconfiguracion de Parametros de Configuracion
    reconfigureParameters();
}

Parameters::~Parameters()
{
}

void Parameters::readParameters()
{
    qDebug() << "Loading parameter Sdgs configuration";

    QSettings settings(SdgsFileParameters, QSettings::IniFormat);

    //Parametros Generales
    strGeneralVersion = getParam(&settings, "GENERAL/GENERAL_VERSION", "").toString();

    //Parametros de Logger
    strLoggerLevel = getParam(&settings, "LOGGER/LOGGER_LEVEL", "INFO").toString();
    strLoggerFile = getParam(&settings, "LOGGER/LOGGER_FILE", "log/debug.log").toString();

    //Parametros de comunicaciones para peticiones y respuestas
    intSnmpPortNms = getParam(&settings, "SNMP/PORT_NMS", "161").toInt();
    intSnmpPortAgent = getParam(&settings, "SNMP/PORT_AGENT", "10500").toInt();
    intSnmpTrapPortNms = getParam(&settings, "TRAP/PORT_NMS", "162").toInt();
    intSnmpTrapPortAgent =  getParam(&settings, "TRAP/PORT_AGENT", "162").toInt();

    //Parametros de configuracion de Traps
    strSnmpTrapAddressNms = getParam(&settings, "TRAP/ADDRESS_NMS", "127.0.0.1").toString().split(" ");
    strSnmpTrapAddressAgent = getParam(&settings, "TRAP/ADDRESS_AGENT", "127.0.0.1").toString();
}

QVariant Parameters::getParam(QSettings *settings, QString strParam, QString strDefault)
{
    //Declaracion de Variables
    QVariant varValue;

    //Obtiene el Parametro
    varValue = settings->value(strParam, strDefault).toString();

    qDebug() << "Parametro [" << strParam << "]" << varValue;

    return varValue;
}

void Parameters::reconfigureParameters()
{

    //Establece el Nivel de Log
    //if (strLoggerLevel == QString("DEBUG")) CoreSimLogger::logLevel = CoreSimLogger::LogLevelDebug;
    //else if (strLoggerLevel == "INFO") CoreSimLogger::logLevel = CoreSimLogger::LogLevelInfo;
    //else if (strLoggerLevel == "WARNING") CoreSimLogger::logLevel = CoreSimLogger::LogLevelWarning;
    //else if (strLoggerLevel == "ERROR") CoreSimLogger::logLevel = CoreSimLogger::LogLevelError;
    //else if (strLoggerLevel == "CRITICAL") CoreSimLogger::logLevel = CoreSimLogger::LogLevelCritical;
    //else CoreSimLogger::logLevel = CoreSimLogger::LogLevelInfo;
    //LogInfo() << "Reconfigured Parameter [Logger Level:" << strLoggerLevel << "]";
}

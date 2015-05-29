#include "configuration/parameters.h"

#include "configuration/snmpxmlmapper.h"
#include "utils/logger/utilslogger.h"

#include <QSettings>
#include <QVariant>
#include <QDebug>

//Ficheros de Configuracion
const QString XmlFileConfiguration = QString("cfg/coresnmp.cfg.xml");

Parameters *Parameters::snmpparams_;

Parameters::Parameters(QObject *parent) :
    QObject(parent)
{
    //Read configuration file
    mapper_ = new SnmpXmlMapper(this);
    mapper_->parsingCfg(XmlFileConfiguration);

    //Setting values
    properties_ = mapper_->getProperties();
}

Parameters::~Parameters()
{
	mapper_->deleteLater();
}

QString Parameters::get(const QString & str)
{
	return snmpparams_->properties_.find(str).value();
}

QVariant Parameters::getParam(QSettings *settings, QString strParam, QString strDefault)
{
    //Declaracion de Variables
    QVariant varValue;

    //Obtiene el Parametro
    varValue = settings->value(strParam, strDefault).toString();
    LogInfo << "Parametro [" << strParam << "]" << varValue;

    return varValue;
}

#include "utils/logger/utilslogger.h"

#include <QSettings>
#include <QVariant>
#include <QDebug>

#include "configuration.h"
#include "xmlreader.h"

//Ficheros de Configuracion
const QString XmlFileConfiguration = QString("cfg/coresnmp.cfg.xml");

Configuration *Configuration::snmpparams_;

Configuration::Configuration(QObject *parent) :
    QObject(parent)
{
    //Read configuration file
    reader_ = new XmlReader(this);
    reader_->parsingCfg(XmlFileConfiguration);

    //Setting values
    properties_ = reader_->getProperties();
}

Configuration::~Configuration()
{
	reader_->deleteLater();
}

QString Configuration::get(const QString & str)
{
	return snmpparams_->properties_.find(str).value();
}

QVariant Configuration::getParam(QSettings *settings, QString strParam, QString strDefault)
{
    //Declaracion de Variables
    QVariant varValue;

    //Obtiene el Parametro
    varValue = settings->value(strParam, strDefault).toString();
    LogInfo << "Parametro [" << strParam << "]" << varValue;

    return varValue;
}

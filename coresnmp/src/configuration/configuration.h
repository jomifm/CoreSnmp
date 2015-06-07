#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>

class QSettings;
class QString;
class QVariant;
class XmlReader;

namespace Params
{
	const QString LoggerDialect = QString("logger.dialect");
	const QString LoggerLevel = QString("logger.level");
	const QString LoggerFile = QString("logger.file");

	const QString Version = QString("config.version");
	const QString RoCommunity = QString("config.rocommunity");
	const QString RwCommunity = QString("config.rwcommunity");
	const QString TrapCommunity = QString("config.trapcommunity");

	const QString NmsPort = QString("connection.request.nms.port");
	const QString AgentPort = QString("connection.request.agent.port");

	const QString TrapNmsAddress = QString("connection.trap.nms.address");
	const QString TrapNmsPort = QString("connection.trap.nms.port");
	const QString TrapAgentPort = QString("connection.trap.agent.port");
}

class Configuration : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param
     */
    explicit Configuration(QObject *parent = 0);

    /**
     * Destructor
     */
    virtual ~Configuration();

    /**
     * Obtiene la instancia unica de los Parametros
     * @param poParent
     * @return
     */
    static Configuration *instance(QObject *poParent = 0) {
        if (snmpparams_ == NULL)
            snmpparams_ = new Configuration(poParent);
        return snmpparams_;
    }

    static QString get(const QString & str);

private:
    /**
     * Instancia unica de los Parametros de Configuracion
     */
    static Configuration *snmpparams_;

    /**
     * @brief Lectura de los Parametros de Configuracion
     */
    void readConfiguration();

    /**
     * @brief Obtiene el valor del parametro del fichero settings
     * @param strParam Nombre del Parametro
     * @param strDefault Valor por defecto
     * @return Valor del parametro
     */
    QVariant getParam(QSettings *settings, QString strParam, QString strDefault);

    //Reader configuration xml file
    XmlReader *reader_;

    //Properties Configuration
    QMap<QString,QString> properties_;

signals:

public slots:

};


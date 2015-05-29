#pragma once

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>

class QSettings;
class QString;
class QVariant;
class SnmpXmlMapper;

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

class Parameters : public QObject
{
    Q_OBJECT

public:
    /**
     * Constructor
     * @param
     */
    explicit Parameters(QObject *parent = 0);

    /**
     * Destructor
     */
    virtual ~Parameters();

    /**
     * Obtiene la instancia unica de los Parametros
     * @param poParent
     * @return
     */
    static Parameters *instance(QObject *poParent = 0) {
        if (snmpparams_ == NULL)
            snmpparams_ = new Parameters(poParent);
        return snmpparams_;
    }

    static QString get(const QString & str);

private:
    /**
     * Instancia unica de los Parametros de Configuracion
     */
    static Parameters *snmpparams_;

    /**
     * @brief Lectura de los Parametros de Configuracion
     */
    void readParameters();

    /**
     * @brief Obtiene el valor del parametro del fichero settings
     * @param strParam Nombre del Parametro
     * @param strDefault Valor por defecto
     * @return Valor del parametro
     */
    QVariant getParam(QSettings *settings, QString strParam, QString strDefault);

    //Mapping configuration xml file
    SnmpXmlMapper *mapper_;

    //Properties parameters
    QMap<QString,QString> properties_;

signals:

public slots:

};


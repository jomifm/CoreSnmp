#pragma once

#include <QtCore/QObject>
#include <QStringList>

class QSettings;
class QString;
class QVariant;

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
    static Parameters *create(QObject *poParent = 0) {
        if (snmpparams_ == NULL)
            snmpparams_ = new Parameters(poParent);
        return snmpparams_;
    }

    //Parametros Generales
    static QString getGeneralVersion() { return snmpparams_->strGeneralVersion; }

    //Parametros de Logger
    static QString getLoggerLevel() { return snmpparams_->strLoggerLevel; }
    static QString getLoggerFile() { return snmpparams_->strLoggerFile; }

    //Parametros de comunicaciones para petitiones y respuestas
    static int getSnmpPortNms() { return snmpparams_->intSnmpPortNms; }
    static int getSnmpPortAgent() { return snmpparams_->intSnmpPortAgent; }

    //Parametros de comunicaciones para traps
    static int getSnmpTrapPortNms() { return snmpparams_->intSnmpTrapPortNms; }
    static int getSnmpTrapPortAgent() { return snmpparams_->intSnmpTrapPortAgent; }
    static QStringList getSnmpTrapAddressNms() { return snmpparams_->strSnmpTrapAddressNms; }
    static QString getSnmpTrapAddressAgent() { return snmpparams_->strSnmpTrapAddressAgent; }

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

    /**
     * @brief Reconfigura los Parametros de Configuracion
     */
    void reconfigureParameters();

    //Parametros Generales
    QString strGeneralVersion;

    //Parametros de Logger
    QString strLoggerLevel;
    QString strLoggerFile;

    //Parametros de comunicaciones para peticiones y respuestas
    int intSnmpPortNms;
    int intSnmpPortAgent;

    //Parametros de comunicaciones para Traps
    int intSnmpTrapPortNms;
    int intSnmpTrapPortAgent;
    QStringList strSnmpTrapAddressNms;
    QString strSnmpTrapAddressAgent;

signals:

public slots:

};


#pragma once

#include <snmp/snmpdatatypes.h>

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QStringList>
#include <QtXml/QDomDocument>

class SnmpXmlMapper : public QObject
{
public:
    /**
     * @brief default constructor
     * */
    SnmpXmlMapper(QObject *parent = NULL);

    /**
     * @brief default destructor
     * */
    virtual ~SnmpXmlMapper();

    void parsingCfg(const QString & strFile);
    void mappingMib(const QString & strFile);

    QMap<QString,QString> getProperties() { return mapProperties; }
    QStringList getMappings() { return listMappings; }

    Type::MMibObject getObjects() { return mapObjects; }
    QMap<QString,QString> getNotifications() { return mapNotifications; }
    QMap<QString,QString> getTraps() { return mapTraps; }

private:
	QMap<QString,QString> mapProperties;
	QStringList listMappings;

	Type::MMibObject mapObjects;
	QMap<QString,QString> mapNotifications;
	QMap<QString,QString> mapTraps;

    QDomDocument getDomDocument(const QString & strFile);

    QString searchOids(QString strParent);
    void appendObjectData(const QString & strTab, const QString & strName,
    		const QString & strOid, const QString & strParent, const QString & type,
			const QString & access, const QString & status, const QString & strDesc);
};


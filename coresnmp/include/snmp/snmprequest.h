#pragma once

#include <snmp/snmpdata.h>
#include <snmp/snmpdatatypes.h>

#include <QSharedPointer>

class SnmpRequest : public QObject
{
	Q_OBJECT

public:
	static SnmpRequest *instance(QObject *parent = 0);

	virtual void get(const QStringList &peerList, const QStringList &objectList) = 0;
	virtual void set(const QStringList &peerList, const Type::MSnmpObject &objectMap) = 0;
	virtual void bulk(const QStringList &peerList, const QStringList &objectList) = 0;
	virtual void walk(const QStringList &peerList, const QString &objectId) = 0;

	virtual void request (QSharedPointer<SnmpData> snmpData) = 0;

protected:
	SnmpRequest (QObject *parent = 0) : QObject (parent) {}

Q_SIGNALS:
	void eventSnmpResponse(QSharedPointer<SnmpData> snmpData);
};

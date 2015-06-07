#pragma once

#include <snmp/snmpdata.h>
#include <snmp/snmpdatatypes.h>

#include <QSharedPointer>

class SnmpTrap : public QObject
{
	Q_OBJECT

public:
	static SnmpTrap *instance(QObject *parent = 0);

	virtual void trap(const QString &trapId,
			const QString &addressSrc, const Type::MSnmpObject &objectMap) = 0;

	virtual void trap(const QStringList &peerList, const QString &trapId,
			const QString &addressSrc, const Type::MSnmpObject &objectMap) = 0;

protected:
	SnmpTrap (QObject *parent = 0) : QObject (parent) {}

Q_SIGNALS:
	void eventSnmpReceiveTrap(QSharedPointer<SnmpTrapData>);
};


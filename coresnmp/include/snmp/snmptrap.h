#pragma once

#include "snmp/snmpdatatypes.h"

#include <QObject>
#include <QStringList>

class SnmpTrap : public QObject
{
    Q_OBJECT

public:
	static SnmpTrap *instance(QObject *parent = 0);

    virtual void trap(const QStringList &peerList, const QString &trapId,
    		const QString &addressSrc, const Type::MSnmpObject &objectMap) = 0;

protected:
    SnmpTrap (QObject *parent = 0) : QObject (parent) {}

Q_SIGNALS:
	void eventSnmpReceiveTrap(const QStringList & objectList);
};


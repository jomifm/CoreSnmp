#pragma once

#include <snmp/snmpdatatypes.h>
#include <snmp/snmptrap.h>

class QUdpSocket;

class SnmpTrapPriv : public SnmpTrap
{
    Q_OBJECT

public:
    SnmpTrapPriv(QObject *parent = 0);

private:
    QUdpSocket *socket_;

	void trap(const QString &trapId,
			const QString &addressSrc, const Type::MSnmpObject &objectMap);

    void trap(const QStringList &peerList, const QString &trapId,
    		const QString &addressSrc, const Type::MSnmpObject &objectMap);

private Q_SLOTS:
    void readPendingDatagram();
};


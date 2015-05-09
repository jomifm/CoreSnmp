#pragma once

#include <snmp/snmpdatatypes.h>

#include <QThread>
#include <QSharedPointer>

class SnmpRequest;
class SnmpTrap;
class SnmpData;

class SnmpTest : public QObject
{
	Q_OBJECT

	SnmpRequest *snmpRequest_;
	SnmpTrap *snmpTrap_;

    QThread myThread_;

public:
    SnmpTest(QObject *parent = 0);

    void init();
    void test();

    void testGetRequest();
    void testSetRequest();
    void testWalkRequest();
    void testBulkRequest();
    void testTrap();

public slots:
	void onEventInit();

	void onEventSnmpResponse(QSharedPointer <SnmpData> snmpData);
	void onEventSnmpReceiveTrap(const QStringList & objectList);

Q_SIGNALS:
	void eventInit();

};

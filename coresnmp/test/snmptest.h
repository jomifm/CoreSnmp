#pragma once

#include <snmp/snmpdatatypes.h>

#include <QThread>
#include <QSharedPointer>

class SnmpRequest;
class SnmpTrap;
class SnmpData;
class SnmpTrapData;

class SnmpTest : public QObject
{
	Q_OBJECT

public:
    //Constructor
    SnmpTest(QObject *parent = 0);
    ~SnmpTest();

    //Initialize snmp test and move to thread
    void init();

    //Execute test();
    void test();

private:
	//Snmp request object
	SnmpRequest *snmpRequest_;

	//Snmp trap object
	SnmpTrap *snmpTrap_;

	//Thread for move this class
    QThread myThread_;

    //Test for snmp get-request
    void testGetRequest();

    //Test for snmp set-request
    void testSetRequest();

    //Test for snmp get-next-request (walk)
    void testWalkRequest();

    //Test method for snmp getbulk-request
    void testBulkRequest();

    //Test method to send traps
    void testTrap();

public slots:
	//Process initialization event
	void onEventInit();

	//Performs tasks and actions after receive snmp responses
	void onEventSnmpResponse(QSharedPointer <SnmpData> snmpData);

	//Performs tasks and actions after receive snmp traps
	void onEventSnmpReceiveTrap(QSharedPointer <SnmpTrapData> trapData);

Q_SIGNALS:
	//Event to perform context change sent in initilization
	void eventInit();

public:
    void timerStart();
    void timerStop();

protected:
    int timerId_;
    void timerEvent(QTimerEvent *event);
};

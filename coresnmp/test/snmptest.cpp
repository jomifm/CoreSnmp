#include "snmptest.h"
#include "utils/logger/utilslogger.h"

#include <snmp/snmprequest.h>
#include <snmp/snmptrap.h>

#include "configuration/parameters.h"
#include "types/basic/snmpoctetstring.h"

const QString strOidSysDescr = "1.3.6.1.2.1.1.1";
const QString strOidSysObjectID = "1.3.6.1.2.1.1.2";
const QString strOidSysUpTime = "1.3.6.1.2.1.1.3";
const QString strOidSysContact = "1.3.6.1.2.1.1.4";
const QString strOidSysName = "1.3.6.1.2.1.1.5";
const QString strOidSysLocation = "1.3.6.1.2.1.1.6";
const QString strOidSysServices = "1.3.6.1.2.1.1.7";

const QString strOrg = "1.3";
const QString strTest = "1.3.6.1.2.1.2.2";
const QString strIfDescr = "1.3.6.1.2.1.2.2.1.2";
const QString strIf_test = "1.3.6.1.2.1.2.2.1.3";

SnmpTest::SnmpTest(QObject *parent) : QObject(parent)
{
	//Move the instance to private thread
	moveToThread(&myThread_);
	myThread_.start();

	connect(this, SIGNAL(eventInit()), this, SLOT(onEventInit()));
}

void SnmpTest::init()
{
	SnmpData::setSnmpVersion(Type::SNMPv1);
	SnmpData::setSnmpCommunity("public");

	emit eventInit();
}

void SnmpTest::onEventInit()
{
	snmpRequest_ = SnmpRequest::instance(this);
	snmpRequest_->moveToThread(&myThread_);

	//Register the object for receive messages
	connect(snmpRequest_, SIGNAL(eventSnmpResponse(QSharedPointer<SnmpData>)),
			this, SLOT(onEventSnmpResponse(QSharedPointer<SnmpData>)), Qt::UniqueConnection);

	snmpTrap_ = SnmpTrap::instance(this);
	snmpTrap_->moveToThread(&myThread_);

	//Register the object for receive messages
	connect(snmpTrap_, SIGNAL(eventSnmpReceiveTrap(const QStringList &)),
			this, SLOT(onEventSnmpReceiveTrap(const QStringList &)), Qt::UniqueConnection);

	myThread_.start();

	test();
}

void SnmpTest::test()
{
    testGetRequest();
    //testSetRequest();
    //testWalkRequest();
    //testBulkRequest();
    //testTrap();
}

void SnmpTest::testGetRequest()
{
    LogFuncionBegin;
    LogInfo() << "!Testing Snmp Get!";

    QStringList dstList;
    //for (int i = 0; i < 500; i++)
    dstList << QString("127.0.0.1");
    //dstList << QString("192.168.10.10");

    QStringList objectList;
    objectList << QString("%1.0").arg(strOidSysDescr);
    objectList << QString("%1.0").arg(strOidSysContact);
    objectList << QString("%1.0").arg(strOidSysLocation);

    //for (int i = 0; i < 1000; i++)
    snmpRequest_->get(dstList, objectList);

    LogFuncionEnd;
}

void SnmpTest::testSetRequest()
{
    LogFuncionBegin;
    LogInfo() << "!Testing Snmp Set!";

    Type::MSnmpObject objectList;
    objectList.insert(QString("%1.0").arg(strOidSysLocation), OctetString::create(QString("indra"))->toObject());

    snmpRequest_->set(QStringList() << "127.0.0.1", objectList);

    LogFuncionEnd;
}

void SnmpTest::testWalkRequest()
{
    LogFuncionBegin;
    LogInfo() << "!Testing Snmp Get!";

    QStringList dstList;
    dstList << QString("127.0.0.1");
    //dstList << QString("192.168.10.10");

    //snmpRequest_->walk(dstList, strIfDescr);
    //snmpRequest_->walk(dstList, strTest);
    snmpRequest_->walk(dstList, strOrg);

    LogFuncionEnd;
}

void SnmpTest::testBulkRequest()
{
    LogFuncionBegin;
    LogInfo() << "!Testing Snmp Get!";

    QStringList dstList;
    dstList << QString("127.0.0.1");
    //dstList << QString("192.168.10.10");

    QStringList objectList;
    objectList << QString("%1").arg(strIfDescr);
    objectList << QString("%1").arg(strIf_test);

    snmpRequest_->bulk(dstList, objectList);

    LogFuncionEnd;
}

void SnmpTest::testTrap()
{
    LogFuncionBegin;
    LogInfo() << "!Testing Snmp Trap!";

    Type::MSnmpObject objectList;
    objectList.insert(QString("%1.0").arg(strOidSysLocation), OctetString::create(QString("pepito"))->toObject());

    //snmpTrap_->trap(QStringList() << "127.0.0.1", strOidSysServices, "192.168.1.1", objectList)
    snmpTrap_->trap(Parameters::getSnmpTrapAddressNms(), strOidSysServices, "192.168.1.1", objectList);

    LogFuncionEnd;
}

void SnmpTest::onEventSnmpResponse(QSharedPointer <SnmpData> data)
{
	qDebug() << "Test received response";
	if (data != NULL)
		qDebug() << data.data() << data;

	quint32 cntObj = 0;
	SnmpData *snmpData = data.data();
	Type::MSnmpObject map = snmpData->getValueList();
	qDebug() << "Received objects:" << map.size();
	for (Type::ISnmpObject iter = map.begin(); iter != map.end(); ++iter) {
		SnmpBasicAbstractType *value = static_cast<SnmpBasicAbstractType*>(iter.value());
		qDebug() << " - Object [" << ++cntObj << "]:" << iter.key() << value->toString();
	}
}

void SnmpTest::onEventSnmpReceiveTrap(const QStringList & objectList)
{
	qDebug() << "Test received trap";
	qDebug() << objectList;
}


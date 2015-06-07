#include "test/snmptest.h"
#include "types/basic/snmpoctetstring.h"
#include "utils/logger/utilslogger.h"

#include <snmp/exceptions/snmpexception.h>
#include <snmp/snmprequest.h>
#include <snmp/snmptrap.h>

//Some Oid groups for gets and sets request
const QString strOidSysDescr = "1.3.6.1.2.1.1.1";
const QString strOidSysObjectID = "1.3.6.1.2.1.1.2";
const QString strOidSysUpTime = "1.3.6.1.2.1.1.3";
const QString strOidSysContact = "1.3.6.1.2.1.1.4";
const QString strOidSysName = "1.3.6.1.2.1.1.5";
const QString strOidSysLocation = "1.3.6.1.2.1.1.6";
const QString strOidSysServices = "1.3.6.1.2.1.1.7";

//Some Oid groups for walks and bulks request
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

SnmpTest::~SnmpTest()
{
	timerStop();
}

void SnmpTest::init()
{
	SnmpData::setSnmpVersion(Type::SNMPv1);
	SnmpData::setSnmpRoCommunity("public");
	SnmpData::setSnmpRwCommunity("private");
	SnmpTrapData::setSnmpTrapCommunity("public");

	emit eventInit();
}

void SnmpTest::onEventInit()
{
	timerStart();

	//Snmp request object
	snmpRequest_ = SnmpRequest::instance(this);
	snmpRequest_->moveToThread(&myThread_);

	//Register the object for receive messages
	connect(snmpRequest_, SIGNAL(eventSnmpResponse(QSharedPointer<SnmpData>)),
			this, SLOT(onEventSnmpResponse(QSharedPointer<SnmpData>)), Qt::UniqueConnection);

	//Snmp traps object
	snmpTrap_ = SnmpTrap::instance(this);
	snmpTrap_->moveToThread(&myThread_);

	//Register the object for receive messages
	connect(snmpTrap_, SIGNAL(eventSnmpReceiveTrap(QSharedPointer<SnmpTrapData>)),
			this, SLOT(onEventSnmpReceiveTrap(QSharedPointer<SnmpTrapData>)), Qt::UniqueConnection);

	myThread_.start();

	//Run test
	//test();
	timerEvent(NULL);
}

void SnmpTest::test()
{
	//Testing get request
    //testGetRequest();

	//Testing set request
    //testSetRequest();

	//Testing walk request
    //testWalkRequest();

    //Testing bulk request
    //testBulkRequest();

    //Testing traps
    testTrap();
}

void SnmpTest::testGetRequest()
{
    LogFuncionBegin;
    LogInfo << "!Testing Snmp Get!";

    LogInfo << "First method in get request";

    //Destination list
    QStringList dstList;
    dstList << QString("127.0.0.1");

    //Object list
    QStringList objectList;
    objectList << QString("%1.0").arg(strOidSysDescr);
    objectList << QString("%1.0").arg(strOidSysContact);
    objectList << QString("%1.0").arg(strOidSysLocation);

    //Send get request
    snmpRequest_->get(dstList, objectList);

    LogInfo << "Second method in get request";

    //Create SnmpData object and set request information
    SnmpData *snmpData = SnmpData::create();
    snmpData->setVersion(Type::SNMPv1);
    snmpData->setCommunity(QString("public"));
    snmpData->setType(Type::GetRequestPDU);
	snmpData->setPeerList(QStringList() << "127.0.0.1");
	snmpData->setObjectList(QStringList() <<
			QString("%1.0").arg(strOidSysDescr) <<
			QString("%1.0").arg(strOidSysContact));
    QSharedPointer<SnmpData> data2 = QSharedPointer<SnmpData>(snmpData);

    //Send get request
    snmpRequest_->request(data2);

    LogInfo << "Three method in get request";

    //Create SnmpData object
    QSharedPointer<SnmpData> data3 = QSharedPointer<SnmpData>(SnmpData::create(
    		Type::SNMPv1, QString("public"), Type::GetRequestPDU, QStringList() << "127.0.0.1",
			QStringList() << QString("%1.0").arg(strOidSysDescr) << QString("%1.0").arg(strOidSysContact)));

    //Send get request
    snmpRequest_->request(data3);

    LogFuncionEnd;
}

void SnmpTest::testSetRequest()
{
    LogFuncionBegin;
    LogInfo << "!Testing Snmp Set!";

    Type::MSnmpObject objectList;
    objectList.insert(QString("%1.0").arg(strOidSysLocation),
    		OctetString::create(QString("indra"))->toObject());

    snmpRequest_->set(QStringList() << "127.0.0.1", objectList);

    LogFuncionEnd;
}

void SnmpTest::testWalkRequest()
{
    LogFuncionBegin;
    LogInfo << "!Testing Snmp Walk!";

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
    LogInfo << "!Testing Snmp Bulk!";

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
    LogInfo << "!Testing Snmp Trap!";

    Type::MSnmpObject objectList;
    objectList.insert(QString("%1.0").arg(strOidSysLocation),
    		OctetString::create(QString("mylocation"))->toObject());

    snmpTrap_->trap(strOidSysServices, "192.168.1.1", objectList);

    Type::MSnmpObject objectList2;
    objectList2.insert(QString("%1.0").arg(strOidSysLocation),
    		OctetString::create(QString("otherlocation"))->toObject());
    objectList2.insert(QString("%1.0").arg(strOidSysName),
        		OctetString::create(QString("myname"))->toObject());
    snmpTrap_->trap(QStringList() << "111.1.1.1", strOidSysServices, "127.0.0.1", objectList2);
    snmpTrap_->trap(QStringList() << "127.0.0.1", strOidSysServices, "127.0.0.1", objectList2);

    LogFuncionEnd;
}

void SnmpTest::onEventSnmpResponse(QSharedPointer <SnmpData> data)
{
	//Variables declaration
	quint32 cntObj = 0;
	SnmpData *snmpData = NULL;

	LogInfo << "Test received response";
	if (data != NULL) LogInfo << data.data() << data;
	else throw SnmpException("SnmpData with NULL value in Trap management.");

	//Take snmp data
	snmpData = data.data();

	//Check error in response
	if (snmpData->getErrorMsg() == Type::RequestTimeout)
	{
		LogInfo << "Timeout in snmp request [Id:" << snmpData->getIdRequest() << "]";

		//Process object list what they was requested
		QStringList objectList = snmpData->getObjectList();
		LogInfo << "Expected objects:" << objectList.size();
		for (int i = 0; i < objectList.size(); i++) {
			LogInfo << " - Oid [" << ++cntObj << "]:" << objectList.at(i);
		}
	}
	else
	{
		LogInfo << "Received Snmp response [Id:" << snmpData->getIdRequest() << "]";

		//Process object list received from request
		Type::MSnmpObject map = snmpData->getValueList();
		LogInfo << "Objects data:" << map.size();
		for (Type::ISnmpObject iter = map.begin(); iter != map.end(); ++iter) {
			SnmpBasicAbstractType *value = static_cast<SnmpBasicAbstractType*>(iter.value());
			LogInfo << " - Object [" << ++cntObj << "]:" << iter.key() << value->toString();
		}
	}
}

void SnmpTest::onEventSnmpReceiveTrap(QSharedPointer <SnmpTrapData> trapData)
{
	//Variables declaration
	quint32 cntObj = 0;
	SnmpTrapData *snmpData = NULL;

	LogInfo << "Test received Trap";
	if (trapData != NULL) LogInfo << trapData.data() << trapData;
	else throw SnmpException("SnmpData with NULL value in Trap management.");

	//Take snmp data
	snmpData = trapData.data();

	LogInfo << "Received Snmp Trap" << endl
			<< "  [Enterprise Oid:" << snmpData->getEnterpriseOid() << "]" << endl
			<< "  [Agent Addr:" << snmpData->getAgentAddr() << "]" << endl
			<< "  [Specific Trap:" << snmpData->getAgentAddr() << "]";

	//Process object list received from request
	Type::MSnmpObject map = snmpData->getValueList();
	LogInfo << "Objects data:" << map.size();
	for (Type::ISnmpObject iter = map.begin(); iter != map.end(); ++iter) {
		SnmpBasicAbstractType *value = static_cast<SnmpBasicAbstractType*>(iter.value());
		LogInfo << " - Object [" << ++cntObj << "]:" << iter.key() << value->toString();
	}
}

void SnmpTest::timerStart()
{
	timerId_ = startTimer(10000);
}
void SnmpTest::timerStop()
{
	if (timerId_)
		killTimer(timerId_);
}

void SnmpTest::timerEvent(QTimerEvent *)
{
	//Check retries
	QString str = QString("ps -o %cpu,%mem,lstart,args,comm,cputime,pid,rss,vsz -p %1").arg(getpid());
	str.append(" >> meminfo.txt");
	system(str.toStdString().c_str());

	test();
}


#include "snmpdata_p.h"

#include <QDebug>
#include <QTimerEvent>

quint32 SnmpData::snmpRequestId_ = 0;
Type::Version SnmpData::snmpVersion_ = Type::SNMPv1;
QString SnmpData::snmpRoCommunity_ = QString("public");
QString SnmpData::snmpRwCommunity_ = QString("private");
QString SnmpData::snmpTrapCommunity_ = QString("public");

SnmpData *SnmpData::create(QObject *parent)
{
	SnmpDataPriv *snmpData = NULL;
	snmpData = new SnmpDataPriv(parent);
	snmpData->setVersion(SnmpData::snmpVersion_);
	return snmpData;
}

SnmpData *SnmpData::create(const Type::AbstractType & idType,
		const QStringList &peerList,
		const QStringList &objectList,
		QObject *parent)
{
	SnmpDataPriv *snmpData = NULL;
	snmpData = new SnmpDataPriv(parent);
	snmpData->setVersion(SnmpData::snmpVersion_);
	snmpData->setType(idType);
	snmpData->setPeerList(peerList);
	snmpData->setObjectList(objectList);
	return snmpData;
}

SnmpData *SnmpData::create(const Type::Version & idVersion,
		const Type::AbstractType & idType,
		const QStringList &peerList,
		const QStringList &objectList,
		QObject *parent)
{
	SnmpData *snmpData = NULL;
	snmpData = create(idType, peerList, objectList, parent);
	snmpData->setVersion(idVersion);
	return snmpData;
}

SnmpData *SnmpData::create(const Type::Version & idVersion,
		const QString & strCommunity,
		const Type::AbstractType & idType,
		const QStringList &peerList,
		const QStringList &objectList,
		QObject *parent)
{
	SnmpData *snmpData = NULL;
	snmpData = create(idVersion, idType, peerList, objectList, parent);
	snmpData->setCommunity(strCommunity);
	return snmpData;
}

SnmpData *SnmpData::create(const Type::Version & idVersion,
		const QString & strCommunity,
		const Type::AbstractType & idType,
		const quint32 & intTimeout,
		const quint32 & intRetries,
		const QStringList &peerList,
		const QStringList &objectList,
		QObject *parent)
{
	SnmpData *snmpData = NULL;
	snmpData = create(idVersion, strCommunity, idType, peerList, objectList, parent);
	snmpData->setTimeout(intTimeout);
	snmpData->setRetries(intRetries);
	return snmpData;
}

SnmpData *SnmpData::create(const Type::AbstractType & idType,
		const QStringList & peerList,
		const Type::MSnmpObject & valueList,
		QObject *parent)
{
	SnmpDataPriv *snmpData = NULL;
	snmpData = new SnmpDataPriv(parent);
	snmpData->setVersion(SnmpData::snmpVersion_);
	snmpData->setCommunity("");
	snmpData->setType(idType);
	snmpData->setPeerList(peerList);
	snmpData->setValueList(valueList);
	return snmpData;
}

SnmpDataPriv::SnmpDataPriv(QObject *parent) :
	SnmpData(parent),
	idRequest_(0),
	intVersion_(Type::SNMPv1),
	strCommunity_(""),
	idType_(Type::Unknown),
	intTimeout_(1000),
	intRetries_(2),
	strObject_(""),
	strSourceAddress_(""),
	idErrorMsg_(Type::NoError)
{
	peerList_.clear();
	objectList_.clear();
	valueList_.clear();

	snmpTimerId_ = 0;
	snmpTimerRetries_ = 0;
}

SnmpDataPriv::SnmpDataPriv(Type::ErrorMessage errMsg, QObject *parent) :
	SnmpData(parent),
	idRequest_(0),
	intVersion_(Type::SNMPv1),
	strCommunity_(""),
	idType_(Type::Unknown),
	intTimeout_(1000),
	intRetries_(2),
	strObject_(""),
	strSourceAddress_(""),
	idErrorMsg_(errMsg)
{
	peerList_.clear();
	objectList_.clear();
	valueList_.clear();

	snmpTimerId_ = 0;
	snmpTimerRetries_ = 0;
}

SnmpDataPriv::~SnmpDataPriv()
{
	snmpTimerStop();
}

void SnmpDataPriv::snmpTimerStart()
{
	if (intRetries_ && !snmpTimerId_)
		snmpTimerId_ = startTimer(getTimeout());
}
void SnmpDataPriv::snmpTimerStop()
{
	if (snmpTimerId_)
		killTimer(snmpTimerId_);
	snmpTimerId_ = 0;
	snmpTimerRetries_ = 0;
}

void SnmpDataPriv::timerEvent(QTimerEvent *)
{
	//Check retries
	snmpTimerRetries_++;
	if (snmpTimerRetries_ > intRetries_) {
		snmpTimerStop();
		emit eventSnmpTimeout();
	}
	else emit eventSnmpRetry();
}

//SnmpTrapInfoPriv::SnmpTrapInfoPriv(QObject *parent) :
//	QObject(parent)
//{
//}
//
//SnmpTrapInfoPriv::~SnmpTrapInfoPriv()
//{
//}

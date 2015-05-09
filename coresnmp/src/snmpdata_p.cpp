#include "snmpdata_p.h"

//#ifdef QT_SNMP_DEBUG
#include <QDebug>
//#endif

quint32 SnmpData::snmpRequestId_ = 0;
Type::Version SnmpData::snmpVersion_ = Type::SNMPv1;
QString SnmpData::snmpCommunity_ = QString("public");

SnmpData *SnmpData::create(const Type::AbstractType & idType,
		const QStringList &peerList,
		const QStringList &objectList,
		QObject *parent)
{
	SnmpDataPriv *snmpData = NULL;
	snmpData = new SnmpDataPriv(parent);
	snmpData->setVersion(SnmpData::snmpVersion_);
	snmpData->setCommunity(SnmpData::snmpCommunity_);
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
	snmpData->setSnmpVersion(idVersion);
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
	snmpData->setSnmpCommunity(strCommunity);
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
	snmpData->setCommunity(SnmpData::snmpCommunity_);
	snmpData->setType(idType);
	snmpData->setPeerList(peerList);
	snmpData->setValueList(valueList);
	return snmpData;
}

SnmpDataPriv::SnmpDataPriv(QObject *parent) :
	SnmpData(parent),
	idRequest_(0),
	intVersion_(Type::SNMPv1),
	strCommunity_("public"),
	idType_(Type::Unknown),
	intTimeout_(1000),
	intRetries_(0),
	strObject_(""),
	strSourceAddress_(""),
	idErrorMsg_(Type::NoError)
{
	peerList_.clear();
	objectList_.clear();
	valueList_.clear();
}

SnmpDataPriv::SnmpDataPriv(Type::ErrorMessage errMsg, QObject *parent) :
	SnmpData(parent),
	idRequest_(0),
	intVersion_(Type::SNMPv1),
	strCommunity_("public"),
	idType_(Type::Unknown),
	intTimeout_(1000),
	intRetries_(0),
	strObject_(""),
	strSourceAddress_(""),
	idErrorMsg_(errMsg)
{
	peerList_.clear();
	objectList_.clear();
	valueList_.clear();
}

SnmpDataPriv::~SnmpDataPriv()
{
}

//SnmpTrapInfoPriv::SnmpTrapInfoPriv(QObject *parent) :
//	QObject(parent)
//{
//}
//
//SnmpTrapInfoPriv::~SnmpTrapInfoPriv()
//{
//}

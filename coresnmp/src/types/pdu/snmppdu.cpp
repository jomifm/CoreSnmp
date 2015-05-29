#include "types/pdu/snmppdu.h"

#include "types/basic/snmpinteger.h"
#include "types/basic/snmpobjectid.h"
#include "types/basic/snmpipaddress.h"
#include "types/basic/snmptimerticks.h"
#include "types/pdu/snmpvarbindlist.h"
#include "utils/logger/utilslogger.h"

#include <QDebug>

ProtocolDataUnit::ProtocolDataUnit(Type::AbstractType type, QObject *parent) :
    SnmpBasicAbstractType(type, parent),
    requestId_(new Integer(this)),
    error_(new Integer(this)),
    errorIndex_(new Integer(this)),
	trapId_(new ObjectIdentifier(this)),
	agentAddr_(new IpAddress(this)),
	genericTrap_(new Integer(this)),
	specificTrap_(new Integer(this)),
	timeStamp_(new TimerTicks(this)),
	varbindList_(new VarbindList(this))
{
}

ProtocolDataUnit::ProtocolDataUnit(Type::AbstractType type, Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList) : // #TODO: Make enum for "type"
    SnmpBasicAbstractType(type),
    requestId_(requestId),
    error_(error),
    errorIndex_(errorIndex),
	trapId_(new ObjectIdentifier(this)),
	agentAddr_(new IpAddress(this)),
	genericTrap_(new Integer(this)),
	specificTrap_(new Integer(this)),
	timeStamp_(new TimerTicks(this)),
	varbindList_(varbindList)
{
    requestId_->setParent(this);
    error_->setParent(this);
    errorIndex_->setParent(this);
    varbindList_->setParent(this);
}

ProtocolDataUnit::ProtocolDataUnit(Type::AbstractType type, ObjectIdentifier *trapId,
		IpAddress *agentAddr, Integer *genericTrap, Integer *specificTrap,
		TimerTicks *timeStamp, VarbindList *varbindList) :
    SnmpBasicAbstractType(type),
    requestId_(new Integer(this)),
    error_(new Integer(this)),
    errorIndex_(new Integer(this)),
	trapId_(trapId),
	agentAddr_(agentAddr),
	genericTrap_(genericTrap),
	specificTrap_(specificTrap),
	timeStamp_(timeStamp),
    varbindList_(varbindList)
{
	trapId_->setParent(this);
	agentAddr_->setParent(this);
	genericTrap_->setParent(this);
	specificTrap_->setParent(this);
	timeStamp_->setParent(this);
    varbindList_->setParent(this);
}

ProtocolDataUnit::~ProtocolDataUnit()
{
}

quint32 ProtocolDataUnit::getDataLength() const
{
	//Check type of PDU
	if (getType() == Type::TrapPDU)
		return trapId_->getLength() + agentAddr_->getLength() +
				genericTrap_->getLength() + specificTrap_->getLength() +
				timeStamp_->getLength() + varbindList_->getLength();
	else
		return requestId_->getLength() + error_->getLength() +
    		errorIndex_->getLength() + varbindList_->getLength();
}

QByteArray ProtocolDataUnit::encodeData() const
{
    QByteArray code;

	//Check type of PDU
	if (getType() == Type::TrapPDU) {
		code.append(trapId_->encode());
		code.append(agentAddr_->encode());
		code.append(genericTrap_->encode());
		code.append(specificTrap_->encode());
		code.append(timeStamp_->encode());
		code.append(varbindList_->encode());
	}
	else {
		code.append(requestId_->encode());
		code.append(error_->encode());
		code.append(errorIndex_->encode());
		code.append(varbindList_->encode());
	}

    LogInfo << "Protocol Data Unit Data:" << code.toHex().constData();
    return code;
}

void ProtocolDataUnit::decodeData(QDataStream &inputStream, quint32 length)
{
    Q_UNUSED(length);

	//Check type of PDU
	if (getType() == Type::TrapPDU)
	{
		quint8 trapIdType = 0;
		inputStream >> trapIdType;
		trapId_->decode(inputStream);

		quint8 agentAddrType = 0;
		inputStream >> agentAddrType;
		agentAddr_->decode(inputStream);

		quint8 genericTrapType = 0;
		inputStream >> genericTrapType;
		genericTrap_->decode(inputStream);

		quint8 specificTrapType = 0;
		inputStream >> specificTrapType;
		specificTrap_->decode(inputStream);

		quint8 timeStampType = 0;
		inputStream >> timeStampType;
		timeStamp_->decode(inputStream);

		quint8 varbindListType = 0;
		inputStream >> varbindListType;
		varbindList_->decode(inputStream);
	}
	else
	{
		quint8 requestIdType = 0;
		inputStream >> requestIdType;
		requestId_->decode(inputStream);

		quint8 errorType = 0;
		inputStream >> errorType;
		error_->decode(inputStream);

		quint8 errorIndexType = 0;
		inputStream >> errorIndexType;
		errorIndex_->decode(inputStream);

		quint8 varbindListType = 0;
		inputStream >> varbindListType;
		varbindList_->decode(inputStream);
	}
}

Integer *ProtocolDataUnit::getRequestId() const {
    return requestId_;
}

Integer *ProtocolDataUnit::getError() const {
    return error_;
}

Integer *ProtocolDataUnit::getErrorIndex() const {
    return errorIndex_;
}

ObjectIdentifier *ProtocolDataUnit::getTrapId() const {
    return trapId_;
}

IpAddress *ProtocolDataUnit::getAgentAddr() const {
    return agentAddr_;
}

Integer *ProtocolDataUnit::getGenericTrap() const {
    return genericTrap_;
}

Integer *ProtocolDataUnit::getSpecificTrap() const {
    return specificTrap_;
}

TimerTicks *ProtocolDataUnit::getTimeStamp() const {
    return timeStamp_;
}

VarbindList *ProtocolDataUnit::getVarbindList() const {
    return varbindList_;
}

QString ProtocolDataUnit::toString()
{
	//Variables declaration
	QString strTypePdu = "";
	QString str;

	//Obtiene el tipo de peticion
	if (getType() == Type::GetRequestPDU) strTypePdu = "get-request";
	else if (getType() == Type::GetNextRequestPDU) strTypePdu = "get-next-request";
	else if (getType() == Type::GetResponsePDU) strTypePdu = "get-response";
	else if (getType() == Type::SetRequestPDU) strTypePdu = "set-request";
	else if (getType() == Type::TrapPDU) strTypePdu = "trap";
	else if (getType() == Type::GetBulkRequestPDU) strTypePdu = "getBulkRequest";
	else if (getType() == Type::NotificationPDU) strTypePdu = "sNMPv2-Trap";
	str.append(QString("  data: %1 (%2)\n").arg(strTypePdu).arg(getType() - Type::GetRequestPDU));
	str.append(QString("    %1\n").arg(strTypePdu));
	if (getType() == Type::TrapPDU) {
		str.append(QString("      enterprise: %1 (%2)\n").arg(getTrapId()->toString()).arg(" "));
		str.append(QString("      agent-addr: %1 (%2)\n").arg(getAgentAddr()->getValue()).arg(getAgentAddr()->toString()));
		str.append(QString("      generic-trap: %1\n").arg(getGenericTrap()->toString()));
		str.append(QString("      specific-trap: %1\n").arg(getSpecificTrap()->toString()));
		str.append(QString("      time-stamp: %1\n").arg(getTimeStamp()->toString()));
	}
	else {
		str.append(QString("      request-id: %1\n").arg(getRequestId()->toString()));
		str.append(QString("      error-status: %1\n").arg(getError()->toString()));
		str.append(QString("      error-index: %1\n").arg(getErrorIndex()->toString()));
	}
	str.append(varbindList_->toString());

    return str;
}

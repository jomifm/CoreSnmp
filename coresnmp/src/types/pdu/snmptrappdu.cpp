#include "types/pdu/snmptrappdu.h"

SnmpTrapPdu::SnmpTrapPdu(QObject *parent) :
    ProtocolDataUnit(Type::TrapPDU, parent)
{
}

SnmpTrapPdu::SnmpTrapPdu(ObjectIdentifier *trapId, IpAddress *agentAddr, Integer *genericTrap,
		Integer *specificTrap, TimerTicks *timeStamp, VarbindList *varbindList) :
    ProtocolDataUnit(Type::TrapPDU, trapId, agentAddr, genericTrap, specificTrap, timeStamp, varbindList)
{
}

SnmpNotificationPdu::SnmpNotificationPdu(QObject *parent) :
    ProtocolDataUnit(Type::GetRequestPDU, parent)
{
}

SnmpNotificationPdu::SnmpNotificationPdu(Integer *requestId,
		Integer *error, Integer *errorIndex, VarbindList *varbindList) :
    ProtocolDataUnit(Type::NotificationPDU, requestId, error, errorIndex, varbindList)
{
}

SnmpInformPdu::SnmpInformPdu(QObject *parent) :
    ProtocolDataUnit(Type::InformRequestedPDU, parent)
{
}

SnmpInformPdu::SnmpInformPdu(Integer *requestId,
		Integer *error, Integer *errorIndex, VarbindList *varbindList) :
    ProtocolDataUnit(Type::GetRequestPDU, requestId, error, errorIndex, varbindList)
{
}

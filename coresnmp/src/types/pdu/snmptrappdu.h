#pragma once

#include "types/pdu/snmppdu.h"

class SnmpTrapPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpTrapPdu(QObject *parent = 0);
    SnmpTrapPdu(ObjectIdentifier *trapId, IpAddress *agentAddr, Integer *genericTrap,
    		Integer *specificTrap, TimerTicks *timeStamp, VarbindList *varbindList);
};

class SnmpNotificationPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpNotificationPdu(QObject *parent = 0);
    SnmpNotificationPdu(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList);
};

class SnmpInformPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpInformPdu(QObject *parent = 0);
    SnmpInformPdu(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList);
};

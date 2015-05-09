#pragma once

#include "types/basic/snmpbasicabstracttype.h"

class Integer;
class IpAddress;
class ObjectIdentifier;
class TimerTicks;
class VarbindList;

// #NOTE: True, but it would be ideally if Sequence be parent class.
class ProtocolDataUnit : public SnmpBasicAbstractType
{
    Q_OBJECT

    //Request and response PDU objects
    Integer *requestId_;
    Integer *error_;
    Integer *errorIndex_;

    //Traps PDU objects
    ObjectIdentifier *trapId_;
    IpAddress *agentAddr_;
    Integer *genericTrap_;
    Integer *specificTrap_;
    TimerTicks *timeStamp_;

    //Common PDU varbind list
    VarbindList *varbindList_;

public:
    ProtocolDataUnit(Type::AbstractType type, QObject *parent = 0);
    ProtocolDataUnit(Type::AbstractType type, Integer *requestId,
    		Integer *error, Integer *errorIndex, VarbindList *varbindList);
    ProtocolDataUnit(Type::AbstractType type, ObjectIdentifier *trapId,
    		IpAddress *agentAddr, Integer *genericTrap, Integer *specificTrap,
    		TimerTicks *timeStamp, VarbindList *varbindList);
    ~ProtocolDataUnit();

    //Request and response PDU objects
    Integer *getRequestId() const;
    Integer *getError() const;
    Integer *getErrorIndex() const;

    //Traps PDU objects
    ObjectIdentifier *getTrapId() const;
    IpAddress *getAgentAddr() const;
    Integer *getGenericTrap() const;
    Integer *getSpecificTrap() const;
    TimerTicks *getTimeStamp() const;

    //Common PDU varbind list
    VarbindList *getVarbindList() const;

    QString toString();

private:
    quint32 getDataLength() const;
    QByteArray encodeData() const;
    void decodeData(QDataStream &inputStream, quint32 length);
};

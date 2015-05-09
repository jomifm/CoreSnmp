#pragma once

#include "types/pdu/snmppdu.h"

class SnmpGetRequestPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpGetRequestPdu(QObject *parent = 0);
    SnmpGetRequestPdu(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList);
};

class SnmpGetNextRequestPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpGetNextRequestPdu(QObject *parent = 0);
    SnmpGetNextRequestPdu(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList);
};

class SnmpGetBulkRequestPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpGetBulkRequestPdu(QObject *parent = 0);
    SnmpGetBulkRequestPdu(Integer *requestId, Integer *nonRepeaters, Integer *maxRepetitions, VarbindList *varbindList);
};

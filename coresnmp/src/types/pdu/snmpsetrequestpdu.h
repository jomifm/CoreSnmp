#pragma once

#include "types/pdu/snmppdu.h"

class SnmpSetRequestPdu : public ProtocolDataUnit
{
    Q_OBJECT
public:
    explicit SnmpSetRequestPdu(QObject *parent = 0);
    SnmpSetRequestPdu(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList);
};

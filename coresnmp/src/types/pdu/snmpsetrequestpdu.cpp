#include "types/pdu/snmpsetrequestpdu.h"

SnmpSetRequestPdu::SnmpSetRequestPdu(QObject *parent) :
    ProtocolDataUnit(Type::SetRequestPDU, parent)
{
}

SnmpSetRequestPdu::SnmpSetRequestPdu(Integer *requestId, Integer *error, Integer *errorIndex, VarbindList *varbindList) :
    ProtocolDataUnit(Type::SetRequestPDU, requestId, error, errorIndex, varbindList)
{
}
